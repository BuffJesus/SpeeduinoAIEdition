/**
 * @file test_filtering.cpp
 * @brief Unit tests for sensor filtering and validation functions
 *
 * Validates ADC filtering, sensor range validation, and edge case handling
 * for safety-critical sensor readings. Tests extracted TESTABLE_INLINE_STATIC
 * helpers from sensors.cpp.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "sensors.h"

// External test helpers exposed via TESTABLE_INLINE_STATIC
extern int16_t fastMap10Bit(uint16_t value, int16_t rangeMin, int16_t rangeMax);
extern uint16_t validateFilterMapSensorReading(uint16_t reading, uint8_t alpha, uint16_t prior);
extern bool isValidBaro(uint8_t baro);

// ========================================== MAP Sensor Validation ==========================================

static void test_validateFilterMapSensorReading_valid_readings(void) {
    // Test valid MAP readings are filtered correctly
    // LOW_PASS_FILTER formula: (input * (256 - alpha) + prior * alpha) / 256
    // alpha controls retention of PRIOR value (0=no filter, 255=full smoothing)

    // Alpha=128 (50/50 mix): prior=500, reading=600
    // Expected: (600 * 128 + 500 * 128) / 256 = (76800 + 64000) / 256 = 140800/256 = 550
    TEST_ASSERT_EQUAL_UINT16(550, validateFilterMapSensorReading(600, 128, 500));

    // Alpha=0 (no filter, 100% input): prior=500, reading=600
    // Expected: (600 * 256 + 500 * 0) / 256 = 600
    TEST_ASSERT_EQUAL_UINT16(600, validateFilterMapSensorReading(600, 0, 500));

    // Alpha=240 (heavy smoothing, ~94% prior): prior=100, reading=200
    // Expected: (200 * 16 + 100 * 240) / 256 = (3200 + 24000) / 256 = 27200/256 = 106
    TEST_ASSERT_EQUAL_UINT16(106, validateFilterMapSensorReading(200, 240, 100));
}

static void test_validateFilterMapSensorReading_rejects_invalid_high(void) {
    // VALID_MAP_MAX=1022, readings >=1022 should be rejected, returning prior value
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(1022, 128, 500));
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(1023, 128, 500));
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(UINT16_MAX, 128, 500));
}

static void test_validateFilterMapSensorReading_rejects_invalid_low(void) {
    // VALID_MAP_MIN=2, readings <=2 should be rejected, returning prior value
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(2, 128, 500));
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(1, 128, 500));
    TEST_ASSERT_EQUAL_UINT16(500, validateFilterMapSensorReading(0, 128, 500));
}

static void test_validateFilterMapSensorReading_boundary_values(void) {
    // Test boundary values (just inside valid range)
    // VALID_MAP_MIN=2, VALID_MAP_MAX=1022, so valid range is [3, 1021]

    // Reading=3 (just above min) with alpha=128, prior=500
    // Expected: (3 * 128 + 500 * 128) / 256 = (384 + 64000) / 256 = 64384/256 = 251
    TEST_ASSERT_EQUAL_UINT16(251, validateFilterMapSensorReading(3, 128, 500));

    // Reading=1021 (just below max) with alpha=128, prior=500
    // Expected: (1021 * 128 + 500 * 128) / 256 = (130688 + 64000) / 256 = 194688/256 = 760
    TEST_ASSERT_EQUAL_UINT16(760, validateFilterMapSensorReading(1021, 128, 500));
}

static void test_validateFilterMapSensorReading_filter_strength_variations(void) {
    // Test different filter strengths on same input
    uint16_t reading = 600;
    uint16_t prior = 500;

    // Weak filter (alpha=20, heavy weight on input): (600*236 + 500*20)/256 = (141600+10000)/256 = 592
    TEST_ASSERT_EQUAL_UINT16(592, validateFilterMapSensorReading(reading, 20, prior));

    // Medium filter (alpha=128, 50/50 mix): (600*128 + 500*128)/256 = 140800/256 = 550
    TEST_ASSERT_EQUAL_UINT16(550, validateFilterMapSensorReading(reading, 128, prior));

    // Strong filter (alpha=200, heavy weight on prior): (600*56 + 500*200)/256 = (33600+100000)/256 = 521
    TEST_ASSERT_EQUAL_UINT16(521, validateFilterMapSensorReading(reading, 200, prior));
}

// ========================================== Baro Validation ==========================================

static void test_isValidBaro_normal_range(void) {
    // Normal sea-level pressure range: 95-105 kPa
    TEST_ASSERT_TRUE(isValidBaro(95));
    TEST_ASSERT_TRUE(isValidBaro(100));
    TEST_ASSERT_TRUE(isValidBaro(105));
}

static void test_isValidBaro_extreme_but_valid(void) {
    // BARO_MIN=65 (high altitude ~3500m), BARO_MAX=108 (Siberian High)
    TEST_ASSERT_TRUE(isValidBaro(65));  // Minimum valid (high altitude)
    TEST_ASSERT_TRUE(isValidBaro(70));  // Above minimum
    TEST_ASSERT_TRUE(isValidBaro(106)); // Below maximum
    TEST_ASSERT_TRUE(isValidBaro(108)); // Maximum valid (extreme high pressure)
}

static void test_isValidBaro_rejects_too_low(void) {
    // Values below BARO_MIN=65 are physically impossible (above 3500m altitude)
    TEST_ASSERT_FALSE(isValidBaro(64));
    TEST_ASSERT_FALSE(isValidBaro(50));
    TEST_ASSERT_FALSE(isValidBaro(0));
}

static void test_isValidBaro_rejects_too_high(void) {
    // Values above BARO_MAX=108 are physically impossible
    TEST_ASSERT_FALSE(isValidBaro(109));
    TEST_ASSERT_FALSE(isValidBaro(120));
    TEST_ASSERT_FALSE(isValidBaro(UINT8_MAX));
}

static void test_isValidBaro_boundary_values(void) {
    // Test exact boundary values
    TEST_ASSERT_TRUE(isValidBaro(65));   // Exactly BARO_MIN (valid)
    TEST_ASSERT_FALSE(isValidBaro(64));  // Just below min (invalid)
    TEST_ASSERT_TRUE(isValidBaro(108));  // Exactly BARO_MAX (valid)
    TEST_ASSERT_FALSE(isValidBaro(109)); // Just above max (invalid)
}

// ========================================== fastMap10Bit Edge Cases ==========================================

static void test_fastMap10Bit_zero_range(void) {
    // Zero range (rangeMin == rangeMax) should return rangeMin
    TEST_ASSERT_EQUAL_INT16(100, fastMap10Bit(0, 100, 100));
    TEST_ASSERT_EQUAL_INT16(100, fastMap10Bit(512, 100, 100));
    TEST_ASSERT_EQUAL_INT16(100, fastMap10Bit(1023, 100, 100));
}

static void test_fastMap10Bit_full_ADC_range(void) {
    // Test full 10-bit ADC range [0, 1023]
    // Map to [0, 1000]
    TEST_ASSERT_EQUAL_INT16(0, fastMap10Bit(0, 0, 1000));
    TEST_ASSERT_EQUAL_INT16(500, fastMap10Bit(512, 0, 1000));  // Middle value
    TEST_ASSERT_EQUAL_INT16(999, fastMap10Bit(1023, 0, 1000)); // Max (truncated due to integer division)
}

static void test_fastMap10Bit_negative_to_positive(void) {
    // Map across zero: [-100, 100]
    TEST_ASSERT_EQUAL_INT16(-100, fastMap10Bit(0, -100, 100));
    TEST_ASSERT_EQUAL_INT16(-1, fastMap10Bit(511, -100, 100));  // Just below zero
    TEST_ASSERT_EQUAL_INT16(0, fastMap10Bit(512, -100, 100));   // Zero crossing
    TEST_ASSERT_EQUAL_INT16(99, fastMap10Bit(1023, -100, 100)); // Max
}

static void test_fastMap10Bit_large_range(void) {
    // Test with large range: [-1500, 1500] (3000 total range)
    TEST_ASSERT_EQUAL_INT16(-1500, fastMap10Bit(0, -1500, 1500));
    TEST_ASSERT_EQUAL_INT16(-3, fastMap10Bit(511, -1500, 1500));  // Just below zero
    TEST_ASSERT_EQUAL_INT16(0, fastMap10Bit(512, -1500, 1500));   // Center
    TEST_ASSERT_EQUAL_INT16(1497, fastMap10Bit(1023, -1500, 1500));
}

static void test_fastMap10Bit_precision_loss(void) {
    // Verify integer division truncation behavior
    // Map [0, 1023] to [0, 100]
    // 1023 * 100 / 1024 = 102300 / 1024 = 99.90... = 99 (truncated)
    TEST_ASSERT_EQUAL_INT16(99, fastMap10Bit(1023, 0, 100));

    // Map [0, 1023] to [0, 10]
    // 1023 * 10 / 1024 = 10230 / 1024 = 9.99... = 9 (truncated)
    TEST_ASSERT_EQUAL_INT16(9, fastMap10Bit(1023, 0, 10));
}

// ========================================== Test Suite Entry Point ==========================================

void test_filtering(void) {
  SET_UNITY_FILENAME() {
    // MAP sensor validation tests
    RUN_TEST(test_validateFilterMapSensorReading_valid_readings);
    RUN_TEST(test_validateFilterMapSensorReading_rejects_invalid_high);
    RUN_TEST(test_validateFilterMapSensorReading_rejects_invalid_low);
    RUN_TEST(test_validateFilterMapSensorReading_boundary_values);
    RUN_TEST(test_validateFilterMapSensorReading_filter_strength_variations);

    // Baro validation tests
    RUN_TEST(test_isValidBaro_normal_range);
    RUN_TEST(test_isValidBaro_extreme_but_valid);
    RUN_TEST(test_isValidBaro_rejects_too_low);
    RUN_TEST(test_isValidBaro_rejects_too_high);
    RUN_TEST(test_isValidBaro_boundary_values);

    // fastMap10Bit edge case tests
    RUN_TEST(test_fastMap10Bit_zero_range);
    RUN_TEST(test_fastMap10Bit_full_ADC_range);
    RUN_TEST(test_fastMap10Bit_negative_to_positive);
    RUN_TEST(test_fastMap10Bit_large_range);
    RUN_TEST(test_fastMap10Bit_precision_loss);
  }
}
