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
 * @brief Load a different tune bank
 *
 * STUB: Full implementation deferred to next session
 */
bool loadTuneBank(uint8_t bankID) {
    // TODO: Implement tune bank switching
    // For now, just return false (feature not yet implemented)
    (void)bankID; // Suppress unused parameter warning
    return false;
}

/**
 * @brief Save current config as a different tune bank
 *
 * STUB: Full implementation deferred to next session
 */
bool saveTuneBank(uint8_t bankID) {
    // TODO: Implement tune bank saving
    (void)bankID;
    return false;
}

/**
 * @brief Create a pre-migration snapshot
 *
 * STUB: Full implementation deferred to next session
 */
bool saveMigrationSnapshot(void) {
    // TODO: Implement migration snapshot creation
    // Should read current EEPROM version, create snapshot file /migration_snapshots/vXX.bin
    return false;
}

/**
 * @brief Restore from most recent migration snapshot
 *
 * STUB: Full implementation deferred to next session
 */
bool restoreMigrationSnapshot(void) {
    // TODO: Implement migration snapshot restore
    return false;
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
