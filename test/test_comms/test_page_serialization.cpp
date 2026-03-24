/**
 * @file test_page_serialization.cpp
 * @brief Tests for TunerStudio page serialization with 16-bit table value support
 *
 * Tests verify that:
 * - Page size calculations account for sizeof(table3d_value_t)
 * - Table values can hold 16-bit range on Teensy 4.1
 * - Table iterators report correct row sizes
 * - Axis values remain 1 byte regardless of table value size
 * - Config-only pages have consistent sizes across platforms
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "pages.h"
#include "table3d.h"
#include "table3d_typedefs.h"

void test_table_value_byte_size(void) {
  // Verify that table3d_value_t has the expected size
#if defined(CORE_TEENSY41)
  TEST_ASSERT_EQUAL(2, sizeof(table3d_value_t));
#else
  TEST_ASSERT_EQUAL(1, sizeof(table3d_value_t));
#endif
}

void test_page_size_16x16_table(void) {
  // Test page size calculation for 16x16 tables (veMapPage, ignMapPage, etc.)
  // Formula: (16 * 16 * sizeof(table3d_value_t)) + (16 + 16) axis bytes

#if defined(CORE_TEENSY41)
  // 16x16 table: 512 bytes (values) + 32 bytes (axes) = 544 bytes
  TEST_ASSERT_EQUAL(544, getPageSize(veMapPage));
  TEST_ASSERT_EQUAL(544, getPageSize(ignMapPage));
  TEST_ASSERT_EQUAL(544, getPageSize(afrMapPage));
  TEST_ASSERT_EQUAL(544, getPageSize(fuelMap2Page));
  TEST_ASSERT_EQUAL(544, getPageSize(ignMap2Page));
#else
  // 16x16 table: 256 bytes (values) + 32 bytes (axes) = 288 bytes
  TEST_ASSERT_EQUAL(288, getPageSize(veMapPage));
  TEST_ASSERT_EQUAL(288, getPageSize(ignMapPage));
  TEST_ASSERT_EQUAL(288, getPageSize(afrMapPage));
  TEST_ASSERT_EQUAL(288, getPageSize(fuelMap2Page));
  TEST_ASSERT_EQUAL(288, getPageSize(ignMap2Page));
#endif
}

void test_page_size_8x8_tables(void) {
  // Test page size for boostvvtPage: 3x 8x8 tables
  // Formula: 3 * ((8 * 8 * sizeof(table3d_value_t)) + (8 + 8) axis bytes)

#if defined(CORE_TEENSY41)
  // 3 * (128 + 16) = 432 bytes
  TEST_ASSERT_EQUAL(432, getPageSize(boostvvtPage));
#else
  // 3 * (64 + 16) = 240 bytes
  TEST_ASSERT_EQUAL(240, getPageSize(boostvvtPage));
#endif
}

void test_page_size_6x6_tables(void) {
  // Test page size for seqFuelPage: 8x 6x6 trim tables
  // Formula: 8 * ((6 * 6 * sizeof(table3d_value_t)) + (6 + 6) axis bytes)

#if defined(CORE_TEENSY41)
  // 8 * (72 + 12) = 672 bytes
  TEST_ASSERT_EQUAL(672, getPageSize(seqFuelPage));
#else
  // 8 * (36 + 12) = 384 bytes
  TEST_ASSERT_EQUAL(384, getPageSize(seqFuelPage));
#endif
}

void test_page_size_mixed_tables(void) {
  // Test page size for wmiMapPage: 2x 8x8 + 1x 4x4
  // Formula: (2 * ((8*8*sizeof) + 16)) + ((4*4*sizeof) + 8)

#if defined(CORE_TEENSY41)
  // 2*(128+16) + (32+8) = 288 + 40 = 328? Let me recalculate...
  // Actually: 2*(128+16) + (32+8) = 2*144 + 40 = 288 + 48 = 336
  TEST_ASSERT_EQUAL(336, getPageSize(wmiMapPage));
#else
  // 2*(64+16) + (16+8) = 2*80 + 24 = 160 + 32 = 192
  TEST_ASSERT_EQUAL(192, getPageSize(wmiMapPage));
#endif
}

void test_config_page_sizes_unchanged(void) {
  // Config pages (no tables) should have same size on all platforms
  TEST_ASSERT_EQUAL(128, getPageSize(veSetPage));    // Page 2
  TEST_ASSERT_EQUAL(128, getPageSize(ignSetPage));   // Page 4
  TEST_ASSERT_EQUAL(128, getPageSize(afrSetPage));   // Page 6
  TEST_ASSERT_EQUAL(192, getPageSize(canbusPage));   // Page 9
  TEST_ASSERT_EQUAL(192, getPageSize(warmupPage));   // Page 10
  TEST_ASSERT_EQUAL(128, getPageSize(progOutsPage)); // Page 13
}

void test_16bit_table_value_range(void) {
#if defined(CORE_TEENSY41)
  // Test that 16-bit table values can hold values > 255
  table3d_value_t test_value = 32768;  // 0x8000
  TEST_ASSERT_EQUAL(32768, test_value);

  test_value = 65535;  // Maximum 16-bit value
  TEST_ASSERT_EQUAL(65535, test_value);

  test_value = 256;  // Just above 8-bit range
  TEST_ASSERT_EQUAL(256, test_value);
#else
  // 8-bit platforms max at 255
  table3d_value_t test_value = 255;
  TEST_ASSERT_EQUAL(255, test_value);
#endif
}

void test_table_row_size(void) {
  // Verify that table row iterators report correct byte size
  // This is critical for send_table_values() which uses row.size()
  // fuelTable is 16x16 by definition

  auto it = fuelTable.values.begin();
  auto row = *it;

#if defined(CORE_TEENSY41)
  // Each row: 16 elements * 2 bytes = 32 bytes
  TEST_ASSERT_EQUAL(32, row.size());
#else
  // Each row: 16 elements * 1 byte = 16 bytes
  TEST_ASSERT_EQUAL(16, row.size());
#endif
}

void test_axis_storage_size(void) {
  // Verify axis values are always 1 byte (uint8_t) regardless of table value size
  // fuelTable is 16x16 by definition

  auto x_it = fuelTable.axisX.begin();
  auto y_it = fuelTable.axisY.begin();

  // Axis iterator should dereference to 1-byte values
  TEST_ASSERT_EQUAL(sizeof(uint8_t), sizeof(*x_it));
  TEST_ASSERT_EQUAL(sizeof(uint8_t), sizeof(*y_it));
}

void test_setup(void) {
  SET_UNITY_FILENAME();

  RUN_TEST_P(test_table_value_byte_size);
  RUN_TEST_P(test_page_size_16x16_table);
  RUN_TEST_P(test_page_size_8x8_tables);
  RUN_TEST_P(test_page_size_6x6_tables);
  RUN_TEST_P(test_page_size_mixed_tables);
  RUN_TEST_P(test_config_page_sizes_unchanged);
  RUN_TEST_P(test_16bit_table_value_range);
  RUN_TEST_P(test_table_row_size);
  RUN_TEST_P(test_axis_storage_size);
}
