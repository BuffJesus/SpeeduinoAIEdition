/**
 * @file test_comms_helpers.cpp
 * @brief Unit tests for legacy comms helper functions
 *
 * Tests pure helper functions from comms_legacy.cpp:
 * - isLegacyCommandByte: validates legacy TunerStudio command bytes
 * - decodeLegacyPageIdentifier: hex ASCII to byte conversion (0-9, a-f, A-F)
 * - shouldRouteSecondaryTunerStudioToPrimary: secondary serial routing logic
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "comms_secondary.h"

// External functions from comms_legacy.cpp
extern bool isLegacyCommandByte(byte highByte, bool allowLegacyComms);
extern byte decodeLegacyPageIdentifier(byte rawPageValue);
extern bool shouldRouteSecondaryTunerStudioToPrimary(uint8_t secondaryProtocol, uint8_t status5);

// Additional pure helpers declared in comms_legacy.h (included via comms_secondary.h above)
// Pull them in explicitly for clarity
#include "comms_legacy.h"

// ========================================== isLegacyCommandByte Tests ==========================================

static void test_isLegacyCommandByte_F_always_allowed(void) {
    // 'F' command is always a legacy command regardless of allowLegacyComms setting
    TEST_ASSERT_TRUE(isLegacyCommandByte('F', false));
    TEST_ASSERT_TRUE(isLegacyCommandByte('F', true));
}

static void test_isLegacyCommandByte_uppercase_letters_when_allowed(void) {
    // Uppercase A-Z (except F, already tested) should be valid when allowLegacyComms is true
    TEST_ASSERT_TRUE(isLegacyCommandByte('A', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('B', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('G', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('S', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('Z', true));
}

static void test_isLegacyCommandByte_lowercase_letters_when_allowed(void) {
    // Lowercase a-z should be valid when allowLegacyComms is true
    TEST_ASSERT_TRUE(isLegacyCommandByte('a', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('n', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('z', true));
}

static void test_isLegacyCommandByte_question_mark_when_allowed(void) {
    // '?' is a special legacy command character
    TEST_ASSERT_TRUE(isLegacyCommandByte('?', true));
}

static void test_isLegacyCommandByte_rejected_when_not_allowed(void) {
    // Non-'F' commands should be rejected when allowLegacyComms is false
    TEST_ASSERT_FALSE(isLegacyCommandByte('A', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte('Z', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte('a', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte('z', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte('?', false));
}

static void test_isLegacyCommandByte_invalid_characters(void) {
    // Characters outside A-Z, a-z, and '?' should always be rejected
    TEST_ASSERT_FALSE(isLegacyCommandByte('0', true));
    TEST_ASSERT_FALSE(isLegacyCommandByte('9', true));
    TEST_ASSERT_FALSE(isLegacyCommandByte(' ', true));
    TEST_ASSERT_FALSE(isLegacyCommandByte('@', true));  // Before 'A'
    TEST_ASSERT_FALSE(isLegacyCommandByte('[', true));  // After 'Z'
    TEST_ASSERT_FALSE(isLegacyCommandByte('`', true));  // Before 'a'
    TEST_ASSERT_FALSE(isLegacyCommandByte('{', true));  // After 'z'
    TEST_ASSERT_FALSE(isLegacyCommandByte(0x00, true)); // Null
    TEST_ASSERT_FALSE(isLegacyCommandByte(0xFF, true)); // High byte
}

// ========================================== decodeLegacyPageIdentifier Tests ==========================================

static void test_decodeLegacyPageIdentifier_decimal_digits(void) {
    // ASCII '0'-'9' should decode to 0-9
    TEST_ASSERT_EQUAL_UINT8(0, decodeLegacyPageIdentifier('0'));
    TEST_ASSERT_EQUAL_UINT8(1, decodeLegacyPageIdentifier('1'));
    TEST_ASSERT_EQUAL_UINT8(5, decodeLegacyPageIdentifier('5'));
    TEST_ASSERT_EQUAL_UINT8(9, decodeLegacyPageIdentifier('9'));
}

static void test_decodeLegacyPageIdentifier_lowercase_hex(void) {
    // ASCII 'a'-'f' should decode to 10-15
    TEST_ASSERT_EQUAL_UINT8(10, decodeLegacyPageIdentifier('a'));
    TEST_ASSERT_EQUAL_UINT8(11, decodeLegacyPageIdentifier('b'));
    TEST_ASSERT_EQUAL_UINT8(12, decodeLegacyPageIdentifier('c'));
    TEST_ASSERT_EQUAL_UINT8(13, decodeLegacyPageIdentifier('d'));
    TEST_ASSERT_EQUAL_UINT8(14, decodeLegacyPageIdentifier('e'));
    TEST_ASSERT_EQUAL_UINT8(15, decodeLegacyPageIdentifier('f'));
}

static void test_decodeLegacyPageIdentifier_uppercase_hex(void) {
    // ASCII 'A'-'F' should decode to 10-15
    TEST_ASSERT_EQUAL_UINT8(10, decodeLegacyPageIdentifier('A'));
    TEST_ASSERT_EQUAL_UINT8(11, decodeLegacyPageIdentifier('B'));
    TEST_ASSERT_EQUAL_UINT8(12, decodeLegacyPageIdentifier('C'));
    TEST_ASSERT_EQUAL_UINT8(13, decodeLegacyPageIdentifier('D'));
    TEST_ASSERT_EQUAL_UINT8(14, decodeLegacyPageIdentifier('E'));
    TEST_ASSERT_EQUAL_UINT8(15, decodeLegacyPageIdentifier('F'));
}

static void test_decodeLegacyPageIdentifier_invalid_returns_unchanged(void) {
    // Invalid characters should be returned unchanged (pass-through behavior)
    TEST_ASSERT_EQUAL_UINT8('G', decodeLegacyPageIdentifier('G'));
    TEST_ASSERT_EQUAL_UINT8('g', decodeLegacyPageIdentifier('g'));
    TEST_ASSERT_EQUAL_UINT8(' ', decodeLegacyPageIdentifier(' '));
    TEST_ASSERT_EQUAL_UINT8('@', decodeLegacyPageIdentifier('@'));
    TEST_ASSERT_EQUAL_UINT8(0x00, decodeLegacyPageIdentifier(0x00));
    TEST_ASSERT_EQUAL_UINT8(0xFF, decodeLegacyPageIdentifier(0xFF));
}

static void test_decodeLegacyPageIdentifier_boundary_values(void) {
    // Test characters just outside valid hex ranges
    TEST_ASSERT_EQUAL_UINT8('/', decodeLegacyPageIdentifier('/'));  // Before '0'
    TEST_ASSERT_EQUAL_UINT8(':', decodeLegacyPageIdentifier(':'));  // After '9'
    TEST_ASSERT_EQUAL_UINT8('@', decodeLegacyPageIdentifier('@'));  // Before 'A'
    TEST_ASSERT_EQUAL_UINT8('`', decodeLegacyPageIdentifier('`'));  // Before 'a'
}

// ========================================== shouldRouteSecondaryTunerStudioToPrimary Tests ==========================================

static void test_shouldRouteSecondaryTunerStudioToPrimary_both_conditions_met(void) {
    // Should route when protocol is TunerStudio AND bit is set
    uint8_t status5 = 0;
    BIT_SET(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS);
    TEST_ASSERT_TRUE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, status5));
}

static void test_shouldRouteSecondaryTunerStudioToPrimary_wrong_protocol(void) {
    // Should not route when protocol is not TunerStudio, even if bit is set
    uint8_t status5 = 0;
    BIT_SET(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS);
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_GENERIC_FIXED, status5));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_MSDROID, status5));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_REALDASH, status5));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(255, status5));
}

static void test_shouldRouteSecondaryTunerStudioToPrimary_bit_not_set(void) {
    // Should not route when bit is not set, even if protocol is TunerStudio
    uint8_t status5 = 0;
    BIT_CLEAR(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS);
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, status5));
}

static void test_shouldRouteSecondaryTunerStudioToPrimary_neither_condition_met(void) {
    // Should not route when neither condition is met
    uint8_t status5 = 0;
    BIT_CLEAR(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS);
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_GENERIC_FIXED, status5));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(0, status5));
}

static void test_shouldRouteSecondaryTunerStudioToPrimary_other_bits_set(void) {
    // Should still work correctly when other bits in status5 are set
    uint8_t status5 = 0xFF; // All bits set
    TEST_ASSERT_TRUE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, status5));

    status5 = 0xFF;
    BIT_CLEAR(status5, BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS); // Clear only the relevant bit
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, status5));
}

// ========================================== calculateLegacySendProgress Tests ==========================================

static void test_calculateLegacySendProgress_full_packet_single_send(void) {
    // Sending exactly the full packet in one go: nextOffset = offset+itemsSent, remainingLength = 0
    LegacySendProgress p = calculateLegacySendProgress(0U, 64U, 64U);
    TEST_ASSERT_EQUAL_UINT8(64U, p.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(0U,  p.remainingLength);
}

static void test_calculateLegacySendProgress_partial_send(void) {
    // Sending 16 of 64 bytes starting at offset 0: next=16, remaining=48
    LegacySendProgress p = calculateLegacySendProgress(0U, 64U, 16U);
    TEST_ASSERT_EQUAL_UINT8(16U, p.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(48U, p.remainingLength);
}

static void test_calculateLegacySendProgress_with_nonzero_offset(void) {
    // Resume: offset=16, packetLength=64, itemsSent=16 → next=32, remaining=48
    LegacySendProgress p = calculateLegacySendProgress(16U, 64U, 16U);
    TEST_ASSERT_EQUAL_UINT8(32U, p.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(48U, p.remainingLength);
}

static void test_calculateLegacySendProgress_clamps_at_uint8_max(void) {
    // nextOffset overflow clamps to UINT8_MAX (255)
    LegacySendProgress p = calculateLegacySendProgress(250U, 10U, 10U);
    TEST_ASSERT_EQUAL_UINT8(255U, p.nextOffset);
    // remainingLength: packetLength(10) - itemsSent(10) = 0
    TEST_ASSERT_EQUAL_UINT8(0U, p.remainingLength);
}

static void test_calculateLegacySendProgress_items_exceed_packet(void) {
    // If itemsSent > packetLength, remainingLength = 0 (not negative)
    LegacySendProgress p = calculateLegacySendProgress(0U, 10U, 20U);
    TEST_ASSERT_EQUAL_UINT8(20U, p.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(0U,  p.remainingLength);
}

// ========================================== getLegacyVersionResponse Tests ==========================================

static void test_getLegacyVersionResponse_Q_returns_speeduino_string(void) {
    const char *resp = getLegacyVersionResponse('Q');
    TEST_ASSERT_NOT_NULL(resp);
    // Must start with "speeduino" (lowercase)
    TEST_ASSERT_EQUAL_UINT8('s', resp[0]);
    TEST_ASSERT_EQUAL_UINT8('p', resp[1]);
    TEST_ASSERT_EQUAL_UINT8('e', resp[2]);
}

static void test_getLegacyVersionResponse_S_returns_Speeduino_string(void) {
    const char *resp = getLegacyVersionResponse('S');
    TEST_ASSERT_NOT_NULL(resp);
    // Must start with "Speeduino" (uppercase)
    TEST_ASSERT_EQUAL_UINT8('S', resp[0]);
}

static void test_getLegacyVersionResponse_unknown_returns_null(void) {
    TEST_ASSERT_NULL(getLegacyVersionResponse('X'));
    TEST_ASSERT_NULL(getLegacyVersionResponse('A'));
    TEST_ASSERT_NULL(getLegacyVersionResponse(0));
}

// ========================================== isLegacyOutputChannelsCommandSupported Tests ==========================================

static void test_isLegacyOutputChannelsCommandSupported_valid(void) {
    TEST_ASSERT_TRUE(isLegacyOutputChannelsCommandSupported(LEGACY_SEND_OUTPUT_CHANNELS_CMD));
}

static void test_isLegacyOutputChannelsCommandSupported_invalid(void) {
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(LEGACY_FIXED_OUTPUT_CHANNELS_CMD));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(LEGACY_NEW_OUTPUT_CHANNELS_CMD));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(0U));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(0xFFU));
}

// ========================================== getLegacyVersionResponseCommand Tests ==========================================

static void test_getLegacyVersionResponseCommand_non_S_passthrough(void) {
    // For any command other than 'S', returns the command unchanged
    TEST_ASSERT_EQUAL_UINT8('A', getLegacyVersionResponseCommand('A', false, 0U));
    TEST_ASSERT_EQUAL_UINT8('Q', getLegacyVersionResponseCommand('Q', false, 0U));
    TEST_ASSERT_EQUAL_UINT8('r', getLegacyVersionResponseCommand('r', false, 0U));
}

static void test_getLegacyVersionResponseCommand_S_primary_msdroid_returns_Q(void) {
    // Primary port + MSDroid → version string via 'Q'
    TEST_ASSERT_EQUAL_UINT8('Q', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_MSDROID));
}

static void test_getLegacyVersionResponseCommand_S_primary_tunerstudio_returns_Q(void) {
    // Primary port + TunerStudio protocol → version string via 'Q'
    TEST_ASSERT_EQUAL_UINT8('Q', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_TUNERSTUDIO));
}

static void test_getLegacyVersionResponseCommand_S_primary_generic_returns_S(void) {
    // Primary port + generic protocol → 'S'
    TEST_ASSERT_EQUAL_UINT8('S', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_GENERIC_FIXED));
    TEST_ASSERT_EQUAL_UINT8('S', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_GENERIC_INI));
}

// ========================================== Test Suite Entry Point ==========================================

void test_comms_helpers(void) {
  SET_UNITY_FILENAME() {
    // isLegacyCommandByte tests
    RUN_TEST(test_isLegacyCommandByte_F_always_allowed);
    RUN_TEST(test_isLegacyCommandByte_uppercase_letters_when_allowed);
    RUN_TEST(test_isLegacyCommandByte_lowercase_letters_when_allowed);
    RUN_TEST(test_isLegacyCommandByte_question_mark_when_allowed);
    RUN_TEST(test_isLegacyCommandByte_rejected_when_not_allowed);
    RUN_TEST(test_isLegacyCommandByte_invalid_characters);

    // decodeLegacyPageIdentifier tests
    RUN_TEST(test_decodeLegacyPageIdentifier_decimal_digits);
    RUN_TEST(test_decodeLegacyPageIdentifier_lowercase_hex);
    RUN_TEST(test_decodeLegacyPageIdentifier_uppercase_hex);
    RUN_TEST(test_decodeLegacyPageIdentifier_invalid_returns_unchanged);
    RUN_TEST(test_decodeLegacyPageIdentifier_boundary_values);

    // shouldRouteSecondaryTunerStudioToPrimary tests
    RUN_TEST(test_shouldRouteSecondaryTunerStudioToPrimary_both_conditions_met);
    RUN_TEST(test_shouldRouteSecondaryTunerStudioToPrimary_wrong_protocol);
    RUN_TEST(test_shouldRouteSecondaryTunerStudioToPrimary_bit_not_set);
    RUN_TEST(test_shouldRouteSecondaryTunerStudioToPrimary_neither_condition_met);
    RUN_TEST(test_shouldRouteSecondaryTunerStudioToPrimary_other_bits_set);

    // calculateLegacySendProgress tests (Phase 4 Slice D)
    RUN_TEST(test_calculateLegacySendProgress_full_packet_single_send);
    RUN_TEST(test_calculateLegacySendProgress_partial_send);
    RUN_TEST(test_calculateLegacySendProgress_with_nonzero_offset);
    RUN_TEST(test_calculateLegacySendProgress_clamps_at_uint8_max);
    RUN_TEST(test_calculateLegacySendProgress_items_exceed_packet);

    // getLegacyVersionResponse tests (Phase 4 Slice D)
    RUN_TEST(test_getLegacyVersionResponse_Q_returns_speeduino_string);
    RUN_TEST(test_getLegacyVersionResponse_S_returns_Speeduino_string);
    RUN_TEST(test_getLegacyVersionResponse_unknown_returns_null);

    // isLegacyOutputChannelsCommandSupported tests (Phase 4 Slice D)
    RUN_TEST(test_isLegacyOutputChannelsCommandSupported_valid);
    RUN_TEST(test_isLegacyOutputChannelsCommandSupported_invalid);

    // getLegacyVersionResponseCommand tests (Phase 4 Slice D)
    RUN_TEST(test_getLegacyVersionResponseCommand_non_S_passthrough);
    RUN_TEST(test_getLegacyVersionResponseCommand_S_primary_msdroid_returns_Q);
    RUN_TEST(test_getLegacyVersionResponseCommand_S_primary_tunerstudio_returns_Q);
    RUN_TEST(test_getLegacyVersionResponseCommand_S_primary_generic_returns_S);
  }
}
