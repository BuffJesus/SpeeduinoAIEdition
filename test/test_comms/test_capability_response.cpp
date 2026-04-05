/**
 * @file test_capability_response.cpp
 * @brief FW-003: Unit tests for buildCapabilityResponse().
 *
 * Tests verify the fixed 39-byte capability packet layout so that any future
 * layout change is caught immediately and the schema_version can be bumped
 * deliberately.
 */

#include <unity.h>
#include "../test_utils.h"
#include "comms_legacy.h"
#include "globals.h"
#include "live_data_map.h"
#include "logger.h"
#include "pages.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

static void fill_cap_buf(uint8_t (&buf)[CAPABILITY_RESPONSE_SIZE])
{
  memset(buf, 0xFFU, sizeof(buf));
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

void test_capability_response_size(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  fill_cap_buf(buf);
  const size_t returned = buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT(CAPABILITY_RESPONSE_SIZE, returned);
}

void test_capability_response_returns_zero_for_undersized_buffer(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE - 1U];
  memset(buf, 0U, sizeof(buf));
  const size_t returned = buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT(0U, returned);
}

void test_capability_response_schema_version(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT8(CAPABILITY_SCHEMA_VERSION, buf[0]);
}

void test_capability_response_board_id_is_stable_id_for_dropbear(void)
{
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = static_cast<byte>(BOARD_ID_DROPBEAR_T41);

  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_DROPBEAR_T41), buf[1]);

  configPage2.pinMapping = originalPinMapping;
}

void test_capability_response_board_id_is_unknown_for_unrecognised_mapping(void)
{
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = 7U; // not a named board

  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_UNKNOWN), buf[1]);

  configPage2.pinMapping = originalPinMapping;
}

void test_capability_response_capability_flags_match_globals(void)
{
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = 0U;

  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT8(getBoardCapabilityFlags(0U), buf[2]);

  configPage2.pinMapping = originalPinMapping;
}

void test_capability_response_output_channel_size_little_endian(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));

  const uint16_t reported = static_cast<uint16_t>(buf[4])
                          | (static_cast<uint16_t>(buf[5]) << 8U);
  TEST_ASSERT_EQUAL_UINT16(static_cast<uint16_t>(LOG_ENTRY_SIZE), reported);
}

void test_capability_response_output_channel_version(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_EQUAL_UINT8(1U, buf[6]);
}

void test_capability_response_signature_starts_at_byte_7(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));

  // The signature field occupies bytes 7-38 (32 bytes).
  // It must be null-terminated somewhere within those 32 bytes.
  bool found_null = false;
  for (size_t i = 7U; i < 7U + CAPABILITY_SIGNATURE_BYTES; ++i)
  {
    if (buf[i] == 0U) { found_null = true; break; }
  }
  TEST_ASSERT_TRUE(found_null);
}

void test_capability_response_signature_matches_q_command(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));

  const char *expected = getLegacyVersionResponse('Q');
  TEST_ASSERT_NOT_NULL(expected);

  // Signature bytes start at offset 7; compare as a C-string.
  const char *actual = reinterpret_cast<const char *>(buf + 7U);
  TEST_ASSERT_EQUAL_STRING(expected, actual);
}

void test_capability_response_feature_flag_u16p2_clear_by_default(void)
{
  // On AVR test harness TS_EXPERIMENTAL_NATIVE_U16_PAGE2 is not defined,
  // so bit 0 must be clear.
#if !defined(TS_EXPERIMENTAL_NATIVE_U16_PAGE2)
  const byte originalPinMapping = configPage2.pinMapping;
  configPage2.pinMapping = 0U; // not DropBear

  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_BITS_LOW(CAP_FEATURE_U16P2, buf[3]);

  configPage2.pinMapping = originalPinMapping;
#else
  TEST_IGNORE_MESSAGE("U16P2 compile flag active — skip default-clear check");
#endif
}

void test_capability_response_feature_flag_runtime_status_a(void)
{
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));

#if (LOG_ENTRY_SIZE) >= 148
  TEST_ASSERT_BITS_HIGH(CAP_FEATURE_RUNTIME_STATUS_A, buf[3]);
#else
  TEST_ASSERT_BITS_LOW(CAP_FEATURE_RUNTIME_STATUS_A, buf[3]);
#endif
}

void test_capability_response_total_size_constant(void)
{
  // Layout sanity: schema(1) + board_id(1) + caps(1) + features(1) +
  //                oc_size(2) + oc_version(1) + signature(32) = 39
  TEST_ASSERT_EQUAL_UINT(39U, CAPABILITY_RESPONSE_SIZE);
  TEST_ASSERT_EQUAL_UINT(32U, CAPABILITY_SIGNATURE_BYTES);
}

// ---------------------------------------------------------------------------
// FW-006: live-data contract metadata
// ---------------------------------------------------------------------------

void test_och_offset_board_capability_flags_value(void)
{
  TEST_ASSERT_EQUAL_UINT16(130U, OCH_OFFSET_BOARD_CAPABILITY_FLAGS);
}

void test_och_offset_flash_health_status_value(void)
{
  TEST_ASSERT_EQUAL_UINT16(131U, OCH_OFFSET_FLASH_HEALTH_STATUS);
}

void test_och_offset_runtime_status_a_value(void)
{
  TEST_ASSERT_EQUAL_UINT16(147U, OCH_OFFSET_RUNTIME_STATUS_A);
}

void test_och_offsets_within_packet_size(void)
{
  // Validate using LIVE_DATA_MAP_SIZE (compile-time guarded against LOG_ENTRY_SIZE
  // in production builds; UNIT_TEST builds use LOG_ENTRY_SIZE=1 so test manually).
  TEST_ASSERT_TRUE(OCH_OFFSET_BOARD_CAPABILITY_FLAGS < LIVE_DATA_MAP_SIZE);
  TEST_ASSERT_TRUE(OCH_OFFSET_FLASH_HEALTH_STATUS    < LIVE_DATA_MAP_SIZE);
  TEST_ASSERT_TRUE(OCH_OFFSET_RUNTIME_STATUS_A       < LIVE_DATA_MAP_SIZE);
}

void test_capability_response_flash_health_clear_on_non_teensy41(void)
{
  // On AVR test harness CORE_TEENSY41 is not defined, so CAP_FEATURE_FLASH_HEALTH
  // must be clear.
#if !defined(CORE_TEENSY41)
  uint8_t buf[CAPABILITY_RESPONSE_SIZE];
  buildCapabilityResponse(buf, sizeof(buf));
  TEST_ASSERT_BITS_LOW(CAP_FEATURE_FLASH_HEALTH, buf[3]);
#else
  TEST_IGNORE_MESSAGE("Teensy41 flash health depends on runtime SPI state");
#endif
}

// ---------------------------------------------------------------------------
// FW-004: getStableBoardId() tests
// ---------------------------------------------------------------------------

void test_stable_board_id_dropbear_maps_correctly(void)
{
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_DROPBEAR_T41),
                          static_cast<uint8_t>(getStableBoardId(55U)));
}

void test_stable_board_id_bear_cub_maps_correctly(void)
{
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_BEAR_CUB),
                          static_cast<uint8_t>(getStableBoardId(56U)));
}

void test_stable_board_id_unknown_for_zero(void)
{
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_UNKNOWN),
                          static_cast<uint8_t>(getStableBoardId(0U)));
}

void test_stable_board_id_unknown_for_arbitrary_value(void)
{
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_UNKNOWN),
                          static_cast<uint8_t>(getStableBoardId(7U)));
}

void test_stable_board_id_dropbear_value_matches_pin_layout_alias(void)
{
  // PIN_LAYOUT_DROPBEAR must equal BOARD_ID_DROPBEAR_T41 so existing comparison
  // sites (pages.cpp, board_teensy41.cpp) remain correct without code changes.
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_DROPBEAR_T41), PIN_LAYOUT_DROPBEAR);
}

void test_stable_board_id_bear_cub_value_matches_pin_layout_alias(void)
{
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(BOARD_ID_BEAR_CUB), PIN_LAYOUT_BEAR_CUB);
}

// ---------------------------------------------------------------------------
// Entry point (called from test_comms main test_setup)
// ---------------------------------------------------------------------------

void test_capability_response_setup(void)
{
  SET_UNITY_FILENAME() {
    // FW-003: capability packet structure
    RUN_TEST_P(test_capability_response_size);
    RUN_TEST_P(test_capability_response_returns_zero_for_undersized_buffer);
    RUN_TEST_P(test_capability_response_schema_version);
    RUN_TEST_P(test_capability_response_board_id_is_stable_id_for_dropbear);
    RUN_TEST_P(test_capability_response_board_id_is_unknown_for_unrecognised_mapping);
    RUN_TEST_P(test_capability_response_capability_flags_match_globals);
    RUN_TEST_P(test_capability_response_output_channel_size_little_endian);
    RUN_TEST_P(test_capability_response_output_channel_version);
    RUN_TEST_P(test_capability_response_signature_starts_at_byte_7);
    RUN_TEST_P(test_capability_response_signature_matches_q_command);
    RUN_TEST_P(test_capability_response_feature_flag_u16p2_clear_by_default);
    RUN_TEST_P(test_capability_response_feature_flag_runtime_status_a);
    RUN_TEST_P(test_capability_response_total_size_constant);
    // FW-006: live-data contract metadata
    RUN_TEST_P(test_och_offset_board_capability_flags_value);
    RUN_TEST_P(test_och_offset_flash_health_status_value);
    RUN_TEST_P(test_och_offset_runtime_status_a_value);
    RUN_TEST_P(test_och_offsets_within_packet_size);
    RUN_TEST_P(test_capability_response_flash_health_clear_on_non_teensy41);
    // FW-004: stable board identifier
    RUN_TEST_P(test_stable_board_id_dropbear_maps_correctly);
    RUN_TEST_P(test_stable_board_id_bear_cub_maps_correctly);
    RUN_TEST_P(test_stable_board_id_unknown_for_zero);
    RUN_TEST_P(test_stable_board_id_unknown_for_arbitrary_value);
    RUN_TEST_P(test_stable_board_id_dropbear_value_matches_pin_layout_alias);
    RUN_TEST_P(test_stable_board_id_bear_cub_value_matches_pin_layout_alias);
  }
}
