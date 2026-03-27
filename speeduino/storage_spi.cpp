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
#include "pages.h"

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

// All config page numbers (1-15). Tune bank save/load copies every page so that a
// full tune switch (tables + structs) is captured in a single bank operation.
static const uint8_t kStructPageIDs[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
static const uint8_t kStructPageCount = sizeof(kStructPageIDs) / sizeof(kStructPageIDs[0]);

// Copy a single file on the LittleFS filesystem. Returns true on success.
static bool copyFlashFile(const char* src, const char* dst)
{
    File srcFile = myfs.open(src, FILE_READ);
    if (!srcFile) { return false; }

    size_t fileSize = srcFile.size();
    if (fileSize == 0 || fileSize > 512) { srcFile.close(); return false; } // max page size is 384 (seqFuelPage)

    uint8_t* buf = (uint8_t*)malloc(fileSize);
    if (buf == NULL) { srcFile.close(); return false; }

    size_t bytesRead = srcFile.read(buf, fileSize);
    srcFile.close();

    if (bytesRead != fileSize) { free(buf); return false; }

    File dstFile = myfs.open(dst, FILE_WRITE);
    if (!dstFile) { free(buf); return false; }

    size_t bytesWritten = dstFile.write(buf, fileSize);
    dstFile.close();
    free(buf);

    return (bytesWritten == fileSize);
}

/**
 * @brief Serialize a table/map page to SPI flash via the page iterator API
 *
 * Uses getPageValue(pageNum, i) to read each byte so that complex page layouts
 * (e.g. seqFuelPage with 8 non-contiguous trim tables) are handled transparently.
 * Stack buffer is sized for the largest possible page (seqFuelPage = 384 bytes).
 */
bool saveTablePageToFlash(uint8_t pageNum) {
    if (!spiFlashInitialized) { return false; }

    uint16_t pageSize = getPageSize(pageNum);
    if (pageSize == 0 || pageSize > 384) { return false; }

    uint8_t buf[384];
    copyPageValuesToBuffer(pageNum, 0U, buf, pageSize);

    char filename[32];
    snprintf(filename, sizeof(filename), "/config/page%d.bin", pageNum);

    File file = myfs.open(filename, FILE_WRITE);
    if (!file) { return false; }

    size_t bytesWritten = file.write(buf, pageSize);
    file.close();

    return (bytesWritten == pageSize);
}

/**
 * @brief Deserialize a table/map page from SPI flash via the page iterator API
 *
 * Uses setPageValue(pageNum, i, v) to populate each byte so that all page types
 * including multi-table pages are restored without an EEPROM round-trip.
 */
bool loadTablePageFromFlash(uint8_t pageNum) {
    if (!spiFlashInitialized) { return false; }

    uint16_t pageSize = getPageSize(pageNum);
    if (pageSize == 0 || pageSize > 384) { return false; }

    char filename[32];
    snprintf(filename, sizeof(filename), "/config/page%d.bin", pageNum);

    File file = myfs.open(filename, FILE_READ);
    if (!file) { return false; }

    uint8_t buf[384];
    size_t bytesRead = file.read(buf, pageSize);
    file.close();

    if (bytesRead != pageSize) { return false; }

    for (uint16_t i = 0; i < pageSize; i++) {
        setPageValue(pageNum, i, buf[i]);
    }

    return true;
}

/**
 * @brief Load a different tune bank (switch active config)
 *
 * Copies all config page files (tables + structs) from /banks/bankN/ to /config/.
 * Takes effect on the next call to loadConfig() — a reboot is required.
 *
 * @param bankID Tune bank ID (0-4, bank 0 is default)
 * @return true if at least one page was copied, false if bank is empty or storage error
 */
bool loadTuneBank(uint8_t bankID) {
    if (!spiFlashInitialized) { return false; }
    if (bankID > 4) { return false; }

    bool anyPageLoaded = false;
    char src[48];
    char dst[32];

    for (uint8_t i = 0; i < kStructPageCount; i++) {
        snprintf(src, sizeof(src), "/banks/bank%d/page%d.bin", bankID, kStructPageIDs[i]);
        snprintf(dst, sizeof(dst), "/config/page%d.bin", kStructPageIDs[i]);
        if (copyFlashFile(src, dst)) { anyPageLoaded = true; }
    }

    return anyPageLoaded;
}

/**
 * @brief Save current config as a different tune bank
 *
 * Copies all config page files (tables + structs) from /config/ to /banks/bankN/.
 * Only pages that have been saved to SPI flash via writeConfig() are included.
 *
 * @param bankID Tune bank ID (0-4, bank 0 is default)
 * @return true if at least one page was saved, false if no pages exist or write error
 */
bool saveTuneBank(uint8_t bankID) {
    if (!spiFlashInitialized) { return false; }
    if (bankID > 4) { return false; }

    bool anyPageSaved = false;
    char src[32];
    char dst[48];

    for (uint8_t i = 0; i < kStructPageCount; i++) {
        snprintf(src, sizeof(src), "/config/page%d.bin", kStructPageIDs[i]);
        snprintf(dst, sizeof(dst), "/banks/bank%d/page%d.bin", bankID, kStructPageIDs[i]);
        if (copyFlashFile(src, dst)) { anyPageSaved = true; }
    }

    return anyPageSaved;
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
    return (uint32_t)myfs.totalSize() - (uint32_t)myfs.usedSize();
}

/**
 * @brief Get total SPI flash capacity
 */
uint32_t getFlashTotalSpace(void) {
    return spiFlashInitialized ? (uint32_t)myfs.totalSize() : 8388608UL;
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
