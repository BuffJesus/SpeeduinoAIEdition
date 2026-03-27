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

extern byte getVE1(void);

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

void test_copy_page_values_matches_get_page_value_for_multi_entity_page(void)
{
  byte pageBuffer[240];
  const uint16_t pageSize = getPageSize(boostvvtPage);

  copyPageValuesToBuffer(boostvvtPage, 0U, pageBuffer, pageSize);

  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    TEST_ASSERT_EQUAL_UINT8(getPageValue(boostvvtPage, offset), pageBuffer[offset]);
  }
}

void test_write_page_values_from_buffer_round_trips_multi_entity_page(void)
{
  byte original[240];
  byte modified[240];
  const uint16_t pageSize = getPageSize(boostvvtPage);

  copyPageValuesToBuffer(boostvvtPage, 0U, original, pageSize);
  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    modified[offset] = byte((offset * 3U) & 0xFFU);
  }

  writePageValuesFromBuffer(boostvvtPage, 0U, modified, pageSize);

  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    TEST_ASSERT_EQUAL_UINT8(modified[offset], getPageValue(boostvvtPage, offset));
  }

  writePageValuesFromBuffer(boostvvtPage, 0U, original, pageSize);
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

void test_multi_entity_page_crc_matches_serialized_ts_byte_stream(void)
{
  byte pageBuffer[240];
  const uint16_t pageSize = getPageSize(boostvvtPage);
  copyPageValuesToBuffer(boostvvtPage, 0U, pageBuffer, pageSize);

  FastCRC32 crcCalc;
  const uint32_t expected = ~crcCalc.crc32(pageBuffer, pageSize, false);

  TEST_ASSERT_EQUAL_HEX32(expected, calculatePageCRC32(boostvvtPage));
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

void test_explicit_ts_byte_mode_matches_existing_contract(void)
{
  byte pageBuffer[288];
  const uint16_t legacyPageSize = getPageSize(veMapPage);

  TEST_ASSERT_EQUAL_UINT16(legacyPageSize, getTunerStudioPageSizeForMode(veMapPage, TS_PAGE_SERIALIZATION_CURRENT_BYTES));

  copyTunerStudioPageValuesToBufferForMode(veMapPage, 0U, pageBuffer, legacyPageSize, TS_PAGE_SERIALIZATION_CURRENT_BYTES);
  for (uint16_t offset = 0; offset < legacyPageSize; offset++)
  {
    TEST_ASSERT_EQUAL_UINT8(getPageValue(veMapPage, offset), pageBuffer[offset]);
  }

  TEST_ASSERT_EQUAL_HEX32(calculatePageCRC32ForMode(veMapPage, TS_PAGE_SERIALIZATION_CURRENT_BYTES), calculatePageCRC32(veMapPage));
}

void test_explicit_ts_byte_mode_round_trips_multi_entity_page(void)
{
  byte original[240];
  byte modified[240];
  const uint16_t pageSize = getPageSize(boostvvtPage);

  copyPageValuesToBuffer(boostvvtPage, 0U, original, pageSize);
  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    modified[offset] = byte((offset * 5U) & 0xFFU);
  }

  writeTunerStudioPageValuesFromBufferForMode(boostvvtPage, 0U, modified, pageSize, TS_PAGE_SERIALIZATION_CURRENT_BYTES);

  for (uint16_t offset = 0; offset < pageSize; offset++)
  {
    TEST_ASSERT_EQUAL_UINT8(modified[offset], getPageValue(boostvvtPage, offset));
  }

  writePageValuesFromBuffer(boostvvtPage, 0U, original, pageSize);
}

void test_experimental_native_u16_page2_seam(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;
  TEST_ASSERT_TRUE(isExperimentalNativeU16Page2Enabled());
  TEST_ASSERT_EQUAL_UINT8(TS_PAGE_SERIALIZATION_NATIVE_U16, getTunerStudioPageSerializationMode(veMapPage));
  TEST_ASSERT_EQUAL_UINT16(544U, getTunerStudioPageSize(veMapPage));

  const table3d_value_t original = fuelTable.values.value_at(0U);
  fuelTable.values.value_at(0U) = 0x1234U;

  byte pageBuffer[544];
  copyTunerStudioPageValuesToBuffer(veMapPage, 0U, pageBuffer, getTunerStudioPageSize(veMapPage));
  TEST_ASSERT_EQUAL_UINT8(0x34U, pageBuffer[0]);
  TEST_ASSERT_EQUAL_UINT8(0x12U, pageBuffer[1]);

  byte modifiedValue[2] = {0xCDU, 0xABU};
  writeTunerStudioPageValuesFromBuffer(veMapPage, 0U, modifiedValue, sizeof(modifiedValue));
  TEST_ASSERT_EQUAL_HEX16(0xABCDU, static_cast<uint16_t>(fuelTable.values.value_at(0U)));

  copyTunerStudioPageValuesToBuffer(veMapPage, 0U, pageBuffer, getTunerStudioPageSize(veMapPage));
  FastCRC32 crcCalc;
  const uint32_t expected = ~crcCalc.crc32(pageBuffer, getTunerStudioPageSize(veMapPage), false);
  TEST_ASSERT_EQUAL_HEX32(expected, calculatePageCRC32(veMapPage));

  fuelTable.values.value_at(0U) = original;
  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("Experimental native U16 page-2 seam is compile-time disabled");
#endif
}

void test_experimental_native_u16_page2_layout_matches_ini_offsets(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  const table3d_value_t originalFirstValue = fuelTable.values.value_at(0U);
  const table3d_value_t originalLastValue = fuelTable.values.value_at(255U);
  const table3d_axis_t originalFirstRpmAxis = *fuelTable.axisX.begin();
  const table3d_axis_t originalLastRpmAxis = *fuelTable.axisX.begin().advance(15U);
  const table3d_axis_t originalFirstLoadAxis = *fuelTable.axisY.begin();
  const table3d_axis_t originalLastLoadAxis = *fuelTable.axisY.begin().advance(15U);
  byte pageBuffer[544];

  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;
  TEST_ASSERT_TRUE(isExperimentalNativeU16Page2Enabled());
  TEST_ASSERT_EQUAL_UINT16(544U, getTunerStudioPageSize(veMapPage));

  fuelTable.values.value_at(0U) = 0x1234U;
  fuelTable.values.value_at(255U) = 0xABCDU;
  *fuelTable.axisX.begin() = 1000;
  *fuelTable.axisX.begin().advance(15U) = 2500;
  *fuelTable.axisY.begin() = 40;
  *fuelTable.axisY.begin().advance(15U) = 220;

  copyTunerStudioPageValuesToBuffer(veMapPage, 0U, pageBuffer, sizeof(pageBuffer));

  TEST_ASSERT_EQUAL_UINT8(0x34U, pageBuffer[0]);
  TEST_ASSERT_EQUAL_UINT8(0x12U, pageBuffer[1]);
  TEST_ASSERT_EQUAL_UINT8(0xCDU, pageBuffer[510]);
  TEST_ASSERT_EQUAL_UINT8(0xABU, pageBuffer[511]);
  TEST_ASSERT_EQUAL_UINT8(get_table3d_axis_converter(fuelTable.axisX.begin().get_domain()).to_byte(1000), pageBuffer[512]);
  TEST_ASSERT_EQUAL_UINT8(get_table3d_axis_converter(fuelTable.axisX.begin().get_domain()).to_byte(2500), pageBuffer[527]);
  TEST_ASSERT_EQUAL_UINT8(get_table3d_axis_converter(fuelTable.axisY.begin().get_domain()).to_byte(40), pageBuffer[528]);
  TEST_ASSERT_EQUAL_UINT8(get_table3d_axis_converter(fuelTable.axisY.begin().get_domain()).to_byte(220), pageBuffer[543]);

  configPage2.pinMapping = originalPinMapping;
  fuelTable.values.value_at(0U) = originalFirstValue;
  fuelTable.values.value_at(255U) = originalLastValue;
  *fuelTable.axisX.begin() = originalFirstRpmAxis;
  *fuelTable.axisX.begin().advance(15U) = originalLastRpmAxis;
  *fuelTable.axisY.begin() = originalFirstLoadAxis;
  *fuelTable.axisY.begin().advance(15U) = originalLastLoadAxis;
#else
  TEST_IGNORE_MESSAGE("Experimental native U16 page-2 layout detail is compile-time disabled");
#endif
}

void test_experimental_native_u16_page2_requires_dropbear_runtime_gate(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;

  configPage2.pinMapping = 0U;
  TEST_ASSERT_FALSE(isExperimentalNativeU16Page2Enabled());
  TEST_ASSERT_EQUAL_UINT8(TS_PAGE_SERIALIZATION_CURRENT_BYTES, getTunerStudioPageSerializationMode(veMapPage));
  TEST_ASSERT_EQUAL_UINT16(getPageSize(veMapPage), getTunerStudioPageSize(veMapPage));

  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("Experimental native U16 page-2 runtime gating detail is compile-time disabled");
#endif
}

void test_experimental_native_u16_page2_runtime_consumers_still_narrow_to_byte(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  const table3d_value_t originalValue = fuelTable.values.value_at(0U);
  const table3d_axis_t originalRpmAxis = *fuelTable.axisX.begin();
  const table3d_axis_t originalLoadAxis = *fuelTable.axisY.begin();
  const uint16_t originalRpm = currentStatus.RPM;
  const int16_t originalFuelLoad = currentStatus.fuelLoad;
  const byte originalAlgorithm = configPage2.fuelAlgorithm;

  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;
  configPage2.fuelAlgorithm = LOAD_SOURCE_MAP;
  *fuelTable.axisX.begin() = 1000;
  *fuelTable.axisY.begin() = 40;
  fuelTable.values.value_at(0U) = 850U;
  currentStatus.RPM = 1000U;
  currentStatus.MAP = 40U;
  currentStatus.fuelLoad = 40;

  TEST_ASSERT_TRUE(isExperimentalNativeU16Page2Enabled());
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(850U & 0xFFU), getVE1());

  configPage2.pinMapping = originalPinMapping;
  configPage2.fuelAlgorithm = originalAlgorithm;
  fuelTable.values.value_at(0U) = originalValue;
  *fuelTable.axisX.begin() = originalRpmAxis;
  *fuelTable.axisY.begin() = originalLoadAxis;
  currentStatus.RPM = originalRpm;
  currentStatus.fuelLoad = originalFuelLoad;
#else
  TEST_IGNORE_MESSAGE("Experimental native U16 page-2 runtime narrowing detail is compile-time disabled");
#endif
}

void test_setup(void)
{
  SET_UNITY_FILENAME() {
    RUN_TEST_P(test_table_value_byte_size_matches_platform);
    RUN_TEST_P(test_virtual_page_sizes_remain_byte_serialized);
    RUN_TEST_P(test_page_iterators_match_virtual_page_sizes);
    RUN_TEST_P(test_copy_page_values_matches_get_page_value);
    RUN_TEST_P(test_copy_page_values_matches_get_page_value_for_multi_entity_page);
    RUN_TEST_P(test_write_page_values_from_buffer_round_trips_multi_entity_page);
    RUN_TEST_P(test_page_crc_matches_serialized_ts_byte_stream);
    RUN_TEST_P(test_multi_entity_page_crc_matches_serialized_ts_byte_stream);
    RUN_TEST_P(test_teensy_internal_values_still_serialize_as_single_bytes);
    RUN_TEST_P(test_explicit_ts_byte_mode_matches_existing_contract);
    RUN_TEST_P(test_explicit_ts_byte_mode_round_trips_multi_entity_page);
    RUN_TEST_P(test_experimental_native_u16_page2_seam);
    RUN_TEST_P(test_experimental_native_u16_page2_layout_matches_ini_offsets);
    RUN_TEST_P(test_experimental_native_u16_page2_requires_dropbear_runtime_gate);
    RUN_TEST_P(test_experimental_native_u16_page2_runtime_consumers_still_narrow_to_byte);
  }
}
