#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>
#include "../test_utils.h"

// Test suite for storage_spi module (Teensy 4.1 SPI flash storage)
// Tests LittleFS-backed config storage, tune banks, migration snapshots

void setUp(void) {
    // Setup runs before each test
}

void tearDown(void) {
    // Cleanup runs after each test
}

// Basic tests (implementation deferred - stubs for now)
void test_spi_flash_init_creates_directory_structure(void) {
    TEST_IGNORE_MESSAGE("Stub: test_spi_flash_init_creates_directory_structure not yet implemented");
}

void test_config_page_save_and_load_roundtrip(void) {
    TEST_IGNORE_MESSAGE("Stub: test_config_page_save_and_load_roundtrip not yet implemented");
}

void test_tune_bank_switch_loads_alternate_config(void) {
    TEST_IGNORE_MESSAGE("Stub: test_tune_bank_switch_loads_alternate_config not yet implemented");
}

void test_migration_snapshot_preserves_config_before_update(void) {
    TEST_IGNORE_MESSAGE("Stub: test_migration_snapshot_preserves_config_before_update not yet implemented");
}

void test_flash_corruption_recovery_reformats_on_mount_failure(void) {
    TEST_IGNORE_MESSAGE("Stub: test_flash_corruption_recovery_reformats_on_mount_failure not yet implemented");
}

void test_wear_leveling_distributes_writes_across_blocks(void) {
    TEST_IGNORE_MESSAGE("Stub: test_wear_leveling_distributes_writes_across_blocks not yet implemented");
}

void setup()
{
    UNITY_BEGIN();

    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_spi_flash_init_creates_directory_structure);
        RUN_TEST_P(test_config_page_save_and_load_roundtrip);
        RUN_TEST_P(test_tune_bank_switch_loads_alternate_config);
        RUN_TEST_P(test_migration_snapshot_preserves_config_before_update);
        RUN_TEST_P(test_flash_corruption_recovery_reformats_on_mount_failure);
        RUN_TEST_P(test_wear_leveling_distributes_writes_across_blocks);
    }

    UNITY_END();

#if defined(SIMULATOR)
    cli();
    sleep_enable();
    sleep_cpu();
#endif
}

void loop() {}
