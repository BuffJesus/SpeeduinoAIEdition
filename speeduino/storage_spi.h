#ifndef STORAGE_SPI_H
#define STORAGE_SPI_H

/**
 * @file storage_spi.h
 * @brief SPI flash storage abstraction for Teensy 4.1 (replaces EEPROM)
 *
 * This module provides LittleFS-backed storage on Teensy 4.1's onboard 8MB SPI flash.
 * Benefits over emulated EEPROM:
 * - 8MB capacity vs. ~4KB EEPROM emulation
 * - Tune banks (multiple configs switchable at runtime)
 * - Migration staging (pre-update snapshots for safe rollback)
 * - High-rate diagnostic logging (knock events, sync loss, protection triggers)
 * - Better wear leveling (LittleFS distributes writes across full flash)
 *
 * This is a Teensy 4.1 specific feature. AVR and other platforms continue to use
 * storage.cpp (EEPROM-backed).
 */

#include <stdint.h>
#include <stdbool.h>

// Only compile this module for Teensy 4.1 (has onboard 8MB QSPI flash)
#if defined(CORE_TEENSY41)

/**
 * @brief Initialize SPI flash and LittleFS filesystem
 *
 * Must be called once at startup before any file operations.
 * Formats flash if uninitialized or corrupted.
 *
 * @return true if initialization succeeded, false if flash hardware failure
 */
bool initSPIFlash(void);

/**
 * @brief Load a config page from SPI flash
 *
 * @param configPage Config page number (1-15, matches EEPROM page numbers)
 * @param buffer Destination buffer (must be at least size bytes)
 * @param size Number of bytes to read
 * @return true if load succeeded, false if file not found or read error
 */
bool loadConfigFromFlash(uint8_t configPage, void* buffer, uint16_t size);

/**
 * @brief Save a config page to SPI flash
 *
 * @param configPage Config page number (1-15, matches EEPROM page numbers)
 * @param buffer Source buffer containing config data
 * @param size Number of bytes to write
 * @return true if save succeeded, false if write error or flash full
 */
bool saveConfigToFlash(uint8_t configPage, const void* buffer, uint16_t size);

/**
 * @brief Load a different tune bank (switch active config)
 *
 * Tune banks allow storing multiple complete configs (e.g., pump gas vs. E85 tune)
 * Bank 0 is the default bank (loaded at startup).
 * Banks 1-4 are alternate tunes (switchable via programmable input or serial command).
 *
 * @param bankID Tune bank ID (0-4)
 * @return true if tune bank exists and was loaded, false if bank empty or load error
 */
bool loadTuneBank(uint8_t bankID);

/**
 * @brief Save current config as a different tune bank
 *
 * @param bankID Tune bank ID (0-4)
 * @return true if save succeeded, false if write error or flash full
 */
bool saveTuneBank(uint8_t bankID);

/**
 * @brief Create a pre-migration snapshot of current config
 *
 * Called by updates.cpp before applying EEPROM structure migrations.
 * Allows rollback if migration fails or causes issues.
 * Snapshot is stored in /migration_snapshots/vXX.bin where XX is pre-migration version.
 *
 * @return true if snapshot created, false if write error or flash full
 */
bool saveMigrationSnapshot(void);

/**
 * @brief Restore config from most recent migration snapshot
 *
 * Used for rollback after failed migration.
 *
 * @return true if restore succeeded, false if no snapshot found or read error
 */
bool restoreMigrationSnapshot(void);

/**
 * @brief Get free space remaining on SPI flash
 *
 * @return Number of bytes available for new files (wear-leveling reserves ~10%)
 */
uint32_t getFlashFreeSpace(void);

/**
 * @brief Get total SPI flash capacity
 *
 * @return Total flash size in bytes (8388608 for Teensy 4.1 = 8MB)
 */
uint32_t getFlashTotalSpace(void);

/**
 * @brief Format SPI flash (erase all files)
 *
 * WARNING: Destroys all stored tunes and config. Use only for recovery.
 *
 * @return true if format succeeded, false if flash hardware failure
 */
bool formatSPIFlash(void);

/**
 * @brief Check if SPI flash storage is initialized and healthy
 *
 * @return true if flash is mounted and writable, false if uninitialized or read-only
 */
bool isSPIFlashHealthy(void);

/**
 * @brief Serialize a table/map page to SPI flash via the page iterator API
 *
 * Iterates byte-by-byte using getPageValue() so all page types are handled uniformly,
 * including multi-table pages (e.g. seqFuelPage with 8 non-contiguous trim tables).
 * No EEPROM round-trip is needed; the in-memory structures are read directly.
 *
 * @param pageNum Page number (1-15; valid for any page including struct-only pages)
 * @return true if save succeeded, false if page size is 0 or write error
 */
bool saveTablePageToFlash(uint8_t pageNum);

/**
 * @brief Deserialize a table/map page from SPI flash via the page iterator API
 *
 * Iterates byte-by-byte using setPageValue() so all page types are populated uniformly.
 * Directly updates in-memory table/struct data without an EEPROM round-trip.
 *
 * @param pageNum Page number (1-15; valid for any page including struct-only pages)
 * @return true if load succeeded, false if file not found, size mismatch, or read error
 */
bool loadTablePageFromFlash(uint8_t pageNum);

#endif // CORE_TEENSY41

#endif // STORAGE_SPI_H
