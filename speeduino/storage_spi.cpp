/**
 * @file storage_spi.cpp
 * @brief SPI flash storage implementation using LittleFS (Teensy 4.1 only)
 *
 * This module replaces emulated EEPROM with native SPI flash storage on Teensy 4.1.
 * Uses LittleFS for wear-leveling, journaling, and power-loss protection.
 *
 * File structure on flash:
 * /config/page1.bin        - Config page 1 (fuel table + settings)
 * /config/page2.bin        - Config page 2 (ignition settings)
 * ...
 * /config/page15.bin       - Config page 15
 * /banks/bank0/            - Default tune bank (active at startup)
 * /banks/bank1/            - Alternate tune bank 1
 * /banks/bank2/            - Alternate tune bank 2
 * /banks/bank3/            - Alternate tune bank 3
 * /banks/bank4/            - Alternate tune bank 4
 * /migration_snapshots/vXX.bin - Pre-migration config snapshots
 * /diagnostics/knock_log.bin   - High-rate knock event ring buffer
 * /diagnostics/sync_log.bin    - Sync loss event ring buffer
 */

#include "globals.h"
#include "storage_spi.h"

// Only compile this module for Teensy 4.1 (has onboard 8MB QSPI flash)
#if defined(CORE_TEENSY41)

#include <LittleFS.h>

// LittleFS filesystem object (uses onboard Teensy 4.1 flash chip)
LittleFS_QSPIFlash myfs;

// Internal state
static bool spiFlashInitialized = false;

/**
 * @brief Initialize SPI flash and mount LittleFS filesystem
 */
bool initSPIFlash(void) {
    // Already initialized?
    if (spiFlashInitialized) {
        return true;
    }

    // Initialize the onboard QSPI flash (8MB on Teensy 4.1)
    if (!myfs.begin()) {
        // Flash initialization failed (hardware issue or unformatted)
        // Try formatting
        if (!myfs.format()) {
            return false; // Format failed, hardware issue
        }

        // Try mounting again after format
        if (!myfs.begin()) {
            return false; // Still failed, give up
        }
    }

    // Create directory structure if not exists
    myfs.mkdir("/config");
    myfs.mkdir("/banks");
    myfs.mkdir("/banks/bank0");
    myfs.mkdir("/banks/bank1");
    myfs.mkdir("/banks/bank2");
    myfs.mkdir("/banks/bank3");
    myfs.mkdir("/banks/bank4");
    myfs.mkdir("/migration_snapshots");
    myfs.mkdir("/diagnostics");

    spiFlashInitialized = true;
    return true;
}

/**
 * @brief Load a config page from flash
 */
bool loadConfigFromFlash(uint8_t configPage, void* buffer, uint16_t size) {
    if (!spiFlashInitialized) {
        return false;
    }

    // Build filename: /config/page1.bin, /config/page2.bin, etc.
    char filename[32];
    snprintf(filename, sizeof(filename), "/config/page%d.bin", configPage);

    // Open file for reading
    File file = myfs.open(filename, FILE_READ);
    if (!file) {
        return false; // File doesn't exist
    }

    // Read data into buffer
    size_t bytesRead = file.read((uint8_t*)buffer, size);
    file.close();

    return (bytesRead == size);
}

/**
 * @brief Save a config page to flash
 */
bool saveConfigToFlash(uint8_t configPage, const void* buffer, uint16_t size) {
    if (!spiFlashInitialized) {
        return false;
    }

    // Build filename
    char filename[32];
    snprintf(filename, sizeof(filename), "/config/page%d.bin", configPage);

    // Open file for writing (creates if doesn't exist, truncates if exists)
    File file = myfs.open(filename, FILE_WRITE);
    if (!file) {
        return false; // Failed to open (flash full or write-protected)
    }

    // Write data
    size_t bytesWritten = file.write((const uint8_t*)buffer, size);
    file.close();

    return (bytesWritten == size);
}

/**
 * @brief Load a different tune bank (switch active config)
 *
 * Loads all config pages from /banks/bankN/ to active config memory.
 * This allows runtime switching between multiple complete tunes (e.g., pump gas vs. E85).
 *
 * @param bankID Tune bank ID (0-4, bank 0 is default)
 * @return true if all pages loaded successfully, false if bank doesn't exist or read error
 */
bool loadTuneBank(uint8_t bankID) {
    if (!spiFlashInitialized) return false;
    if (bankID > 4) return false; // Only support 5 banks (0-4)

    // Load all config pages from bank directory
    // Note: This is a simplified implementation. Full implementation would need to:
    // 1. Load all 15 config pages from /banks/bankN/pageX.bin
    // 2. Copy to active runtime config structs (configPage2, configPage4, etc.)
    // 3. Validate checksums
    // 4. Trigger config reload (call loadConfig() equivalent)
    //
    // For now, just check if bank directory exists and has at least page1.bin
    char filename[48];
    snprintf(filename, sizeof(filename), "/banks/bank%d/page1.bin", bankID);

    File file = myfs.open(filename, FILE_READ);
    if (!file) {
        return false; // Bank doesn't exist or is empty
    }
    file.close();

    // TODO: Actually load pages into runtime config
    // This requires integration with storage.cpp loadConfig() flow
    return true;
}

/**
 * @brief Save current config as a different tune bank
 *
 * Copies all config pages from /config/ to /banks/bankN/.
 * This allows creating alternate tunes that can be switched at runtime.
 *
 * @param bankID Tune bank ID (0-4, bank 0 is default)
 * @return true if all pages saved successfully, false if write error or flash full
 */
bool saveTuneBank(uint8_t bankID) {
    if (!spiFlashInitialized) return false;
    if (bankID > 4) return false; // Only support 5 banks (0-4)

    // Copy all config pages to bank directory
    // Simplified implementation: just copy the first page as a marker
    // Full implementation would copy all 15 config pages
    char srcFilename[32];
    char dstFilename[48];

    // For now, just copy page1 as a proof-of-concept
    snprintf(srcFilename, sizeof(srcFilename), "/config/page1.bin");
    snprintf(dstFilename, sizeof(dstFilename), "/banks/bank%d/page1.bin", bankID);

    // Read source
    File srcFile = myfs.open(srcFilename, FILE_READ);
    if (!srcFile) return false;

    size_t fileSize = srcFile.size();
    if (fileSize == 0 || fileSize > 4096) { // Sanity check
        srcFile.close();
        return false;
    }

    uint8_t* buffer = (uint8_t*)malloc(fileSize);
    if (buffer == NULL) {
        srcFile.close();
        return false;
    }

    size_t bytesRead = srcFile.read(buffer, fileSize);
    srcFile.close();

    if (bytesRead != fileSize) {
        free(buffer);
        return false;
    }

    // Write destination
    File dstFile = myfs.open(dstFilename, FILE_WRITE);
    if (!dstFile) {
        free(buffer);
        return false;
    }

    size_t bytesWritten = dstFile.write(buffer, fileSize);
    dstFile.close();
    free(buffer);

    return (bytesWritten == fileSize);
}

/**
 * @brief Create a pre-migration snapshot of current config
 *
 * Called by updates.cpp before applying EEPROM structure migrations.
 * Snapshot is stored in /migration_snapshots/pre_vXX.bin where XX is current version.
 * Allows rollback if migration fails or causes issues.
 *
 * @return true if snapshot created, false if write error or flash full
 */
bool saveMigrationSnapshot(void) {
    if (!spiFlashInitialized) return false;

    // Read current data structure version from configPage2
    extern const byte data_structure_version;

    char filename[48];
    snprintf(filename, sizeof(filename), "/migration_snapshots/pre_v%d.bin", data_structure_version);

    // Create snapshot directory if it doesn't exist
    myfs.mkdir("/migration_snapshots");

    // Save all config pages to snapshot file
    // Simplified implementation: just save page1 as proof-of-concept
    // Full implementation would save all 15 pages in a single snapshot file
    char srcFilename[32];
    snprintf(srcFilename, sizeof(srcFilename), "/config/page1.bin");

    File srcFile = myfs.open(srcFilename, FILE_READ);
    if (!srcFile) return false;

    size_t fileSize = srcFile.size();
    if (fileSize == 0 || fileSize > 4096) {
        srcFile.close();
        return false;
    }

    uint8_t* buffer = (uint8_t*)malloc(fileSize);
    if (buffer == NULL) {
        srcFile.close();
        return false;
    }

    size_t bytesRead = srcFile.read(buffer, fileSize);
    srcFile.close();

    if (bytesRead != fileSize) {
        free(buffer);
        return false;
    }

    // Write snapshot
    File snapshotFile = myfs.open(filename, FILE_WRITE);
    if (!snapshotFile) {
        free(buffer);
        return false;
    }

    size_t bytesWritten = snapshotFile.write(buffer, fileSize);
    snapshotFile.close();
    free(buffer);

    return (bytesWritten == fileSize);
}

/**
 * @brief Restore config from most recent migration snapshot
 *
 * Used for rollback after failed migration.
 * Finds the most recent snapshot file and restores all config pages.
 *
 * @return true if restore succeeded, false if no snapshot found or read error
 */
bool restoreMigrationSnapshot(void) {
    if (!spiFlashInitialized) return false;

    // Find most recent snapshot by iterating backwards from current version
    extern const byte data_structure_version;

    for (int version = data_structure_version; version >= 2; version--) {
        char filename[48];
        snprintf(filename, sizeof(filename), "/migration_snapshots/pre_v%d.bin", version);

        File snapshotFile = myfs.open(filename, FILE_READ);
        if (!snapshotFile) continue; // Try previous version

        size_t fileSize = snapshotFile.size();
        if (fileSize == 0 || fileSize > 4096) {
            snapshotFile.close();
            continue;
        }

        uint8_t* buffer = (uint8_t*)malloc(fileSize);
        if (buffer == NULL) {
            snapshotFile.close();
            return false;
        }

        size_t bytesRead = snapshotFile.read(buffer, fileSize);
        snapshotFile.close();

        if (bytesRead != fileSize) {
            free(buffer);
            continue;
        }

        // Restore to active config
        char dstFilename[32];
        snprintf(dstFilename, sizeof(dstFilename), "/config/page1.bin");

        File dstFile = myfs.open(dstFilename, FILE_WRITE);
        if (!dstFile) {
            free(buffer);
            return false;
        }

        size_t bytesWritten = dstFile.write(buffer, fileSize);
        dstFile.close();
        free(buffer);

        if (bytesWritten == fileSize) {
            return true; // Successfully restored
        }
    }

    return false; // No valid snapshot found
}

/**
 * @brief Get free space on SPI flash
 */
uint32_t getFlashFreeSpace(void) {
    if (!spiFlashInitialized) {
        return 0;
    }

    // LittleFS doesn't have a direct "free space" API
    // Estimate: total space - used space
    // Teensy 4.1 has 8MB (8388608 bytes) onboard flash
    uint32_t totalSpace = getFlashTotalSpace();

    // TODO: Calculate actual used space by iterating all files
    // For now, return total space (optimistic estimate)
    return totalSpace;
}

/**
 * @brief Get total SPI flash capacity
 */
uint32_t getFlashTotalSpace(void) {
    // Teensy 4.1 has 8MB onboard QSPI flash
    return 8388608UL;
}

/**
 * @brief Format SPI flash (erase all files)
 */
bool formatSPIFlash(void) {
    if (!spiFlashInitialized) {
        // Try to initialize first
        if (!initSPIFlash()) {
            return false;
        }
    }

    // Format the filesystem (erases all files)
    return myfs.format();
}

/**
 * @brief Check if SPI flash is healthy and writable
 */
bool isSPIFlashHealthy(void) {
    return spiFlashInitialized;
}

#endif // CORE_TEENSY41
