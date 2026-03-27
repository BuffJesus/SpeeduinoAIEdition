/**
 * @file test_page_serialization.cpp
 * @brief Tests for the current TunerStudio virtual page contract.
 *
 * These tests intentionally lock the current byte-serialized TS page layout in place.
 * Teensy 4.1 may store table values as uint16_t internally, but the TS-visible pages
 * are still serialized as byte-oriented virtual pages.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "page_crc.h"
#include "pages.h"
#include "table3d_typedefs.h"
#include "../../speeduino/src/FastCRC/FastCRC.h"

void test_table_value_byte_size_matches_platform(void)
{
#if defined(CORE_TEENSY41)
  TEST_ASSERT_EQUAL(2, sizeof(table3d_value_t));
#else
  TEST_ASSERT_EQUAL(1, sizeof(table3d_value_t));
#endif
}

void test_virtual_page_sizes_remain_byte_serialized(void)
{
  TEST_ASSERT_EQUAL_UINT16(288U, getPageSize(veMapPage));
  TEST_ASSERT_EQUAL_UINT16(288U, getPageSize(ignMapPage));
  TEST_ASSERT_EQUAL_UINT16(288U, getPageSize(afrMapPage));
  TEST_ASSERT_EQUAL_UINT16(240U, getPageSize(boostvvtPage));
  TEST_ASSERT_EQUAL_UINT16(384U, getPageSize(seqFuelPage));
  TEST_ASSERT_EQUAL_UINT16(288U, getPageSize(fuelMap2Page));
  TEST_ASSERT_EQUAL_UINT16(192U, getPageSize(wmiMapPage));
  TEST_ASSERT_EQUAL_UINT16(288U, getPageSize(ignMap2Page));
}

void test_page_iterators_match_virtual_page_sizes(void)
{
  TEST_ASSERT_EQUAL_UINT16(getPageSize(veMapPage), page_begin(veMapPage).size);
  TEST_ASSERT_EQUAL_UINT16(getPageSize(ignMapPage), page_begin(ignMapPage).size);
  TEST_ASSERT_EQUAL_UINT16(getPageSize(afrMapPage), page_begin(afrMapPage).size);

  uint16_t boostvvtTotal = 0U;
  for (page_iterator_t it = page_begin(boostvvtPage); it.type != End; it = advance(it))
  {
    boostvvtTotal += it.size;
  }
  TEST_ASSERT_EQUAL_UINT16(getPageSize(boostvvtPage), boostvvtTotal);

  uint16_t seqFuelTotal = 0U;
  for (page_iterator_t it = page_begin(seqFuelPage); it.type != End; it = advance(it))
  {
    seqFuelTotal += it.size;
  }
  TEST_ASSERT_EQUAL_UINT16(getPageSize(seqFuelPage), seqFuelTotal);
}

void test_copy_page_values_matches_get_page_value(void)
{
  byte pageBuffer[288];
  const uint16_t pageSize = getPageSize(veMapPage);

  copyPageValuesToBuffer(veMapPage, 0U, pageBuffer, pageSize);

  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    TEST_ASSERT_EQUAL_UINT8(getPageValue(veMapPage, offset), pageBuffer[offset]);
  }
}

void test_page_crc_matches_serialized_ts_byte_stream(void)
{
  byte pageBuffer[288];
  const uint16_t pageSize = getPageSize(veMapPage);
  copyPageValuesToBuffer(veMapPage, 0U, pageBuffer, pageSize);

  FastCRC32 crcCalc;
  const uint32_t expected = ~crcCalc.crc32(pageBuffer, pageSize, false);

  TEST_ASSERT_EQUAL_HEX32(expected, calculatePageCRC32(veMapPage));
}

void test_teensy_internal_values_still_serialize_as_single_bytes(void)
{
#if defined(CORE_TEENSY41)
  const table3d_value_t original = fuelTable.values.value_at(0U);
  fuelTable.values.value_at(0U) = 0x1234U;

  TEST_ASSERT_EQUAL_UINT8(0x34U, getPageValue(veMapPage, 0U));

  fuelTable.values.value_at(0U) = original;
#else
  TEST_IGNORE_MESSAGE("Teensy-only serialization detail");
#endif
}

void test_setup(void)
{
  SET_UNITY_FILENAME() {
    RUN_TEST_P(test_table_value_byte_size_matches_platform);
    RUN_TEST_P(test_virtual_page_sizes_remain_byte_serialized);
    RUN_TEST_P(test_page_iterators_match_virtual_page_sizes);
    RUN_TEST_P(test_copy_page_values_matches_get_page_value);
    RUN_TEST_P(test_page_crc_matches_serialized_ts_byte_stream);
    RUN_TEST_P(test_teensy_internal_values_still_serialize_as_single_bytes);
  }
}
