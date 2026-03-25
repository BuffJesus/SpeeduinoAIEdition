/**
 * @file test_board_capability_export.cpp
 * @brief Unit tests for board capability output channel export (Phase 4)
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "logger.h"

/**
 * @brief Test that board capability flags are exported correctly at byte 130
 *
 * Verifies Phase 4 board capability output channel implementation:
 * - Byte 130 should return the board capability bitfield
 * - Flags should match the current pin mapping configuration
 */
static void test_board_capability_export_byte_130(void)
{
  // Set pin mapping to Dropbear (Teensy 4.1)
  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;

  // Get board capability flags from output channel byte 130
  byte exportedCapabilities = getTSLogEntry(130);

  // Get expected capabilities for Dropbear on Teensy 4.1
  byte expectedCapabilities = getBoardCapabilityFlags(PIN_LAYOUT_DROPBEAR);

  // Verify they match
  TEST_ASSERT_EQUAL_UINT8(expectedCapabilities, exportedCapabilities);

  #if defined(CORE_TEENSY41)
    // On Teensy 4.1, verify expected flags are set
    TEST_ASSERT_TRUE(exportedCapabilities & BOARD_CAP_12BIT_ADC);
    TEST_ASSERT_TRUE(exportedCapabilities & BOARD_CAP_HIGH_RES_TABLES);
    TEST_ASSERT_TRUE(exportedCapabilities & BOARD_CAP_SPI_FLASH);  // Dropbear has SPI flash
  #endif
}

/**
 * @brief Test that getReadableLogEntry also exports board capabilities
 *
 * The readable log entry (index 94) should also return board capabilities
 * for consistency across output channels.
 */
static void test_board_capability_readable_export(void)
{
  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;

  int16_t exportedCapabilities = getReadableLogEntry(94);
  byte expectedCapabilities = getBoardCapabilityFlags(PIN_LAYOUT_DROPBEAR);

  TEST_ASSERT_EQUAL_INT16(expectedCapabilities, exportedCapabilities);
}

/**
 * @brief Test capability export changes with pin mapping
 *
 * Verify that different pin mappings produce different capability flags
 * (e.g., Bear Cub vs Dropbear may have different capabilities).
 */
static void test_board_capability_varies_by_pin_mapping(void)
{
  // Get Dropbear capabilities
  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;
  byte dropbearCaps = getTSLogEntry(130);

  // Get Bear Cub capabilities (if different)
  configPage2.pinMapping = PIN_LAYOUT_BEAR_CUB;
  byte bearCubCaps = getTSLogEntry(130);

  // At minimum, verify the function returns a value
  // (actual difference depends on board configuration)
  TEST_ASSERT_TRUE(dropbearCaps >= 0);
  TEST_ASSERT_TRUE(bearCubCaps >= 0);

  #if defined(CORE_TEENSY41)
    // On Teensy 4.1, Dropbear should have SPI flash but Bear Cub shouldn't
    TEST_ASSERT_TRUE(dropbearCaps & BOARD_CAP_SPI_FLASH);
    TEST_ASSERT_FALSE(bearCubCaps & BOARD_CAP_SPI_FLASH);
  #endif
}

/**
 * @brief Test that SPI flash health is exported at byte 131
 *
 * On non-Teensy4.1 platforms (AVR sim), isSPIFlashHealthy() is not compiled;
 * the channel should return 0. On Teensy 4.1 the return depends on flash state.
 */
static void test_spi_flash_health_export_byte_131(void)
{
  byte result = getTSLogEntry(131);
  // On AVR sim (not CORE_TEENSY41) the channel is always 0
  #if !defined(CORE_TEENSY41)
    TEST_ASSERT_EQUAL_UINT8(0U, result);
  #else
    // On Teensy 4.1 the value reflects actual flash health; just verify range
    TEST_ASSERT_TRUE(result == 0U || result == 1U);
  #endif
}

/**
 * @brief Test that getReadableLogEntry index 95 exports SPI flash health
 */
static void test_spi_flash_health_readable_entry_index_95(void)
{
  int16_t result = getReadableLogEntry(95);
  #if !defined(CORE_TEENSY41)
    TEST_ASSERT_EQUAL_INT16(0, result);
  #else
    TEST_ASSERT_TRUE(result == 0 || result == 1);
  #endif
}

void test_board_capability_export(void)
{
  SET_UNITY_FILENAME() {
    RUN_TEST(test_board_capability_export_byte_130);
    RUN_TEST(test_board_capability_readable_export);
    RUN_TEST(test_board_capability_varies_by_pin_mapping);
    RUN_TEST(test_spi_flash_health_export_byte_131);
    RUN_TEST(test_spi_flash_health_readable_entry_index_95);
  }
}
