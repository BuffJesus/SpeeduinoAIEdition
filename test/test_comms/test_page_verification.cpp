/**
 * @file test_page_verification.cpp
 * @brief FW-005: External page verification path tests.
 *
 * Tests cover:
 *   - parseLegacyCrcRequest  — wire-level request parser
 *   - encodeLegacyCrc32Response — wire-level response encoder
 *   - calculatePageCRC32 / calculatePageCRC32ForMode round-trip
 *   - Mode awareness: CAP_FEATURE_U16P2 flag links to CRC serialization mode
 *
 * The external verification procedure for a tuner is:
 *   1. Issue 'K' capability query to learn CAP_FEATURE_U16P2 flag.
 *   2. Serialize the page locally using the matching mode:
 *        production  -> TS_PAGE_SERIALIZATION_CURRENT_BYTES (byte-oriented)
 *        experimental -> TS_PAGE_SERIALIZATION_NATIVE_U16  (native uint16 LE)
 *   3. Compute CRC32 over the serialized bytes (FastCRC32 convention: ~crc32()).
 *   4. Send legacy 'd' command {0x00, pageNum} and receive 4-byte big-endian CRC32.
 *   5. Compare.
 */

#include <unity.h>
#include "../test_utils.h"
#include "comms_legacy.h"
#include "globals.h"
#include "page_crc.h"
#include "pages.h"
#include "../../speeduino/src/FastCRC/FastCRC.h"

// ---------------------------------------------------------------------------
// parseLegacyCrcRequest
// ---------------------------------------------------------------------------

void test_parse_crc_request_extracts_page_from_byte1(void)
{
  const uint8_t buf[2] = {0x00U, 0x05U}; // byte 0 ignored, page = byte 1
  LegacyCrcRequest request;
  TEST_ASSERT_TRUE(parseLegacyCrcRequest(buf, sizeof(buf), &request));
  TEST_ASSERT_EQUAL_UINT8(5U, request.page);
}

void test_parse_crc_request_byte0_is_ignored(void)
{
  const uint8_t bufA[2] = {0xFFU, 0x03U};
  const uint8_t bufB[2] = {0x00U, 0x03U};
  LegacyCrcRequest reqA, reqB;
  parseLegacyCrcRequest(bufA, sizeof(bufA), &reqA);
  parseLegacyCrcRequest(bufB, sizeof(bufB), &reqB);
  TEST_ASSERT_EQUAL_UINT8(reqA.page, reqB.page);
}

void test_parse_crc_request_rejects_null_buffer(void)
{
  LegacyCrcRequest request;
  TEST_ASSERT_FALSE(parseLegacyCrcRequest(nullptr, 2U, &request));
}

void test_parse_crc_request_rejects_null_output(void)
{
  const uint8_t buf[2] = {0x00U, 0x01U};
  TEST_ASSERT_FALSE(parseLegacyCrcRequest(buf, sizeof(buf), nullptr));
}

void test_parse_crc_request_rejects_short_buffer(void)
{
  const uint8_t buf[1] = {0x01U};
  LegacyCrcRequest request;
  TEST_ASSERT_FALSE(parseLegacyCrcRequest(buf, sizeof(buf), &request));
}

void test_parse_crc_request_accepts_exactly_2_bytes(void)
{
  const uint8_t buf[2] = {0x00U, 0x02U};
  LegacyCrcRequest request;
  TEST_ASSERT_TRUE(parseLegacyCrcRequest(buf, sizeof(buf), &request));
  TEST_ASSERT_EQUAL_UINT8(2U, request.page);
}

// ---------------------------------------------------------------------------
// encodeLegacyCrc32Response
// ---------------------------------------------------------------------------

void test_encode_crc32_response_big_endian(void)
{
  uint8_t buf[4];
  encodeLegacyCrc32Response(0x12345678U, buf);
  TEST_ASSERT_EQUAL_HEX8(0x12U, buf[0]);
  TEST_ASSERT_EQUAL_HEX8(0x34U, buf[1]);
  TEST_ASSERT_EQUAL_HEX8(0x56U, buf[2]);
  TEST_ASSERT_EQUAL_HEX8(0x78U, buf[3]);
}

void test_encode_crc32_response_all_zeros(void)
{
  uint8_t buf[4] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
  encodeLegacyCrc32Response(0x00000000U, buf);
  TEST_ASSERT_EQUAL_HEX8(0x00U, buf[0]);
  TEST_ASSERT_EQUAL_HEX8(0x00U, buf[1]);
  TEST_ASSERT_EQUAL_HEX8(0x00U, buf[2]);
  TEST_ASSERT_EQUAL_HEX8(0x00U, buf[3]);
}

void test_encode_crc32_response_null_buffer_is_safe(void)
{
  // Must not crash or access invalid memory.
  encodeLegacyCrc32Response(0xDEADBEEFU, nullptr);
  TEST_PASS();
}

// ---------------------------------------------------------------------------
// Round-trip: encode(calculatePageCRC32()) is self-consistent
// ---------------------------------------------------------------------------

void test_page_crc_encode_roundtrip_is_stable(void)
{
  // Encode the same CRC twice — the encoded bytes must be identical.
  const uint32_t crc = calculatePageCRC32(veMapPage);
  uint8_t bufA[4], bufB[4];
  encodeLegacyCrc32Response(crc, bufA);
  encodeLegacyCrc32Response(crc, bufB);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(bufA, bufB, 4U);
}

// ---------------------------------------------------------------------------
// Mode awareness: CAP_FEATURE_U16P2 flag ↔ CRC serialization mode
// ---------------------------------------------------------------------------

void test_production_mode_crc_matches_byte_serialized_data(void)
{
  // An external tool serializing with TS_PAGE_SERIALIZATION_CURRENT_BYTES must
  // produce the same CRC as calculatePageCRC32ForMode with that mode.
  byte pageBuffer[288]; // veMapPage is 288 bytes in byte mode
  const uint16_t pageSize = getPageSize(veMapPage);
  copyPageValuesToBuffer(veMapPage, 0U, pageBuffer, pageSize);

  FastCRC32 crcCalc;
  const uint32_t expected = ~crcCalc.crc32(pageBuffer, pageSize, false);

  TEST_ASSERT_EQUAL_HEX32(expected,
    calculatePageCRC32ForMode(veMapPage, TS_PAGE_SERIALIZATION_CURRENT_BYTES));
}

void test_production_mode_capability_flag_implies_byte_crc(void)
{
#if !defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  // On builds without the U16P2 compile flag, CAP_FEATURE_U16P2 must be clear
  // and the page serialization mode must be byte-oriented.
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = 0U;

  uint8_t capBuf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(capBuf, sizeof(capBuf));
  TEST_ASSERT_BITS_LOW(CAP_FEATURE_U16P2, capBuf[3]);

  TEST_ASSERT_EQUAL_UINT8(TS_PAGE_SERIALIZATION_CURRENT_BYTES,
    getTunerStudioPageSerializationMode(veMapPage));

  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("U16P2 compile flag active — check via CORE_TEENSY41 path");
#endif
}

void test_u16p2_mode_crc_differs_from_production_when_values_set(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  const table3d_value_t originalValue = fuelTable.values.value_at(0U);

  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;
  fuelTable.values.value_at(0U) = 0x1234U; // value that differs when serialized as U08 vs U16

  const uint32_t byteCrc = calculatePageCRC32ForMode(veMapPage, TS_PAGE_SERIALIZATION_CURRENT_BYTES);
  const uint32_t u16Crc  = calculatePageCRC32ForMode(veMapPage, TS_PAGE_SERIALIZATION_NATIVE_U16);

  TEST_ASSERT_NOT_EQUAL(byteCrc, u16Crc);

  fuelTable.values.value_at(0U) = originalValue;
  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("U16P2 CRC difference test requires CORE_TEENSY41 + TS_EXPERIMENTAL_NATIVE_U16_PAGE2");
#endif
}

void test_u16p2_mode_capability_flag_implies_native_u16_crc(void)
{
#if defined(CORE_TEENSY41) && defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = PIN_LAYOUT_DROPBEAR;

  // Capability response must set CAP_FEATURE_U16P2
  uint8_t capBuf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(capBuf, sizeof(capBuf));
  TEST_ASSERT_BITS_HIGH(CAP_FEATURE_U16P2, capBuf[3]);

  // The ECU CRC must match what an external tool computes in U16 mode
  byte pageBuffer[544];
  const uint16_t pageSize = getTunerStudioPageSizeForMode(veMapPage, TS_PAGE_SERIALIZATION_NATIVE_U16);
  copyTunerStudioPageValuesToBufferForMode(veMapPage, 0U, pageBuffer, pageSize, TS_PAGE_SERIALIZATION_NATIVE_U16);

  FastCRC32 crcCalc;
  const uint32_t expected = ~crcCalc.crc32(pageBuffer, pageSize, false);

  TEST_ASSERT_EQUAL_HEX32(expected, calculatePageCRC32(veMapPage));

  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("U16P2 CRC/capability link requires CORE_TEENSY41 + TS_EXPERIMENTAL_NATIVE_U16_PAGE2");
#endif
}

// ---------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------

void test_page_verification_setup(void)
{
  SET_UNITY_FILENAME() {
    // parseLegacyCrcRequest
    RUN_TEST_P(test_parse_crc_request_extracts_page_from_byte1);
    RUN_TEST_P(test_parse_crc_request_byte0_is_ignored);
    RUN_TEST_P(test_parse_crc_request_rejects_null_buffer);
    RUN_TEST_P(test_parse_crc_request_rejects_null_output);
    RUN_TEST_P(test_parse_crc_request_rejects_short_buffer);
    RUN_TEST_P(test_parse_crc_request_accepts_exactly_2_bytes);
    // encodeLegacyCrc32Response
    RUN_TEST_P(test_encode_crc32_response_big_endian);
    RUN_TEST_P(test_encode_crc32_response_all_zeros);
    RUN_TEST_P(test_encode_crc32_response_null_buffer_is_safe);
    // Round-trip
    RUN_TEST_P(test_page_crc_encode_roundtrip_is_stable);
    // Mode awareness
    RUN_TEST_P(test_production_mode_crc_matches_byte_serialized_data);
    RUN_TEST_P(test_production_mode_capability_flag_implies_byte_crc);
    RUN_TEST_P(test_u16p2_mode_crc_differs_from_production_when_values_set);
    RUN_TEST_P(test_u16p2_mode_capability_flag_implies_native_u16_crc);
  }
}
