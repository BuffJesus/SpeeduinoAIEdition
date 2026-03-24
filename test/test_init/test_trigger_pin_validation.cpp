/**
 * @file test_trigger_pin_validation.cpp
 * @brief Unit tests for decoder trigger pin interrupt validation (Phase 4)
 *
 * Tests validateTriggerPins() function that ensures decoder trigger pins
 * support external interrupts on platforms with restricted interrupt capabilities
 * (e.g., AVR Mega 2560 has only 6 external interrupt pins: 2, 3, 18, 19, 20, 21).
 *
 * Safety-critical: Engine won't sync if trigger pins aren't interrupt-capable.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "init.h"
#include "decoders.h"

// ========================================== validateTriggerPins Tests ==========================================

static void test_validateTriggerPins_with_valid_primary_pin(void) {
  // Test validation with valid interrupt pin for primary trigger
  #if defined(ARDUINO_ARCH_AVR)
    // On AVR Mega 2560: pin 2 is INT0, a valid external interrupt pin
    pinTrigger = 2;
  #elif defined(CORE_TEENSY41)
    // On Teensy 4.1: all pins are interrupt-capable, use any pin
    pinTrigger = 0;
  #else
    pinTrigger = 2;
  #endif

  // Decoder without secondary trigger
  BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
  configPage10.vvt2Enabled = 0;

  // Clear error flag
  BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

  // Should return true (primary pin valid)
  TEST_ASSERT_TRUE(validateTriggerPins());
  TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
}

static void test_validateTriggerPins_with_invalid_primary_pin(void) {
  // Test validation failure with invalid interrupt pin for primary trigger
  #if defined(ARDUINO_ARCH_AVR)
    // On AVR Mega 2560: pin 4 is NOT an external interrupt pin
    pinTrigger = 4;

    // Decoder without secondary trigger
    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return false and set error flag (primary pin invalid on AVR)
    TEST_ASSERT_FALSE(validateTriggerPins());
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #elif defined(CORE_TEENSY41)
    // On Teensy 4.1: validation is skipped due to unrestricted interrupts
    // Any pin will pass validation
    pinTrigger = 99;

    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (validation skipped on Teensy 4.1)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_with_valid_secondary_pin(void) {
  // Test validation with valid secondary trigger pin
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;  // Valid primary (INT0)
    pinTrigger2 = 3; // Valid secondary (INT1)

    // Decoder WITH secondary trigger
    BIT_SET(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (both pins valid)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #elif defined(CORE_TEENSY41)
    pinTrigger = 0;
    pinTrigger2 = 1;

    BIT_SET(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (all pins valid on Teensy)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_with_invalid_secondary_pin(void) {
  // Test validation failure with invalid secondary trigger pin
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;  // Valid primary (INT0)
    pinTrigger2 = 5; // Invalid secondary (not an external interrupt pin)

    // Decoder WITH secondary trigger
    BIT_SET(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return false and set error flag (secondary pin invalid)
    TEST_ASSERT_FALSE(validateTriggerPins());
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_with_valid_tertiary_pin(void) {
  // Test validation with valid tertiary (VVT2) trigger pin
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;   // Valid primary (INT0)
    pinTrigger3 = 18; // Valid tertiary (INT3)

    // Decoder without secondary, but VVT2 enabled
    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 1;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (primary and tertiary valid)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #elif defined(CORE_TEENSY41)
    pinTrigger = 0;
    pinTrigger3 = 2;

    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 1;
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (all pins valid on Teensy)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_with_invalid_tertiary_pin(void) {
  // Test validation failure with invalid tertiary trigger pin
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;  // Valid primary (INT0)
    pinTrigger3 = 6; // Invalid tertiary (not an external interrupt pin)

    // Decoder without secondary, but VVT2 enabled
    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 1;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return false and set error flag (tertiary pin invalid)
    TEST_ASSERT_FALSE(validateTriggerPins());
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_secondary_not_checked_when_disabled(void) {
  // Test that secondary pin is NOT validated when decoder doesn't use it
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;  // Valid primary (INT0)
    pinTrigger2 = 99; // Invalid secondary (but shouldn't be checked)

    // Decoder WITHOUT secondary trigger
    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (secondary not checked, primary valid)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_tertiary_not_checked_when_disabled(void) {
  // Test that tertiary pin is NOT validated when VVT2 is disabled
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;  // Valid primary (INT0)
    pinTrigger3 = 99; // Invalid tertiary (but shouldn't be checked)

    // Decoder without secondary, VVT2 disabled
    BIT_CLEAR(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 0;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (tertiary not checked, primary valid)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

static void test_validateTriggerPins_all_pins_valid(void) {
  // Test all trigger pins valid
  #if defined(ARDUINO_ARCH_AVR)
    pinTrigger = 2;   // Valid primary (INT0)
    pinTrigger2 = 3;  // Valid secondary (INT1)
    pinTrigger3 = 18; // Valid tertiary (INT3)

    // Decoder with all triggers enabled
    BIT_SET(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 1;

    // Clear error flag
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (all pins valid)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #elif defined(CORE_TEENSY41)
    pinTrigger = 0;
    pinTrigger2 = 1;
    pinTrigger3 = 2;

    BIT_SET(decoderState, BIT_DECODER_HAS_SECONDARY);
    configPage10.vvt2Enabled = 1;
    BIT_CLEAR(currentStatus.engineProtectStatus, PROTECT_IO_ERROR);

    // Should return true (all pins valid on Teensy)
    TEST_ASSERT_TRUE(validateTriggerPins());
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.engineProtectStatus, PROTECT_IO_ERROR));
  #endif
}

// ========================================== Test Suite Entry Point ==========================================

void test_trigger_pin_validation(void) {
  SET_UNITY_FILENAME() {
    // Core validation tests (run on all platforms)
    RUN_TEST(test_validateTriggerPins_with_valid_primary_pin);
    RUN_TEST(test_validateTriggerPins_with_invalid_primary_pin);
    RUN_TEST(test_validateTriggerPins_with_valid_secondary_pin);
    RUN_TEST(test_validateTriggerPins_with_valid_tertiary_pin);
    RUN_TEST(test_validateTriggerPins_all_pins_valid);

    // AVR-specific validation failure tests
    #if defined(ARDUINO_ARCH_AVR)
      RUN_TEST(test_validateTriggerPins_with_invalid_secondary_pin);
      RUN_TEST(test_validateTriggerPins_with_invalid_tertiary_pin);
      RUN_TEST(test_validateTriggerPins_secondary_not_checked_when_disabled);
      RUN_TEST(test_validateTriggerPins_tertiary_not_checked_when_disabled);
    #endif
  }
}
