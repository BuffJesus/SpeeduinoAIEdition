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
#include "sensors_map_structs.h"
#include "storage.h"

// External test helpers exposed via TESTABLE_INLINE_STATIC
extern int16_t fastMap10Bit(uint16_t value, int16_t rangeMin, int16_t rangeMax);
extern uint16_t validateFilterMapSensorReading(uint16_t reading, uint8_t alpha, uint16_t prior);
extern bool isValidBaro(uint8_t baro);
extern uint16_t mapADCToMAP(uint16_t mapADC, int8_t mapMin, uint16_t mapMax);
extern void setMAPValuesFromReadings(const map_adc_readings_t &readings, const config2 &page2, bool useEMAP, statuses &current);
extern void setBaroFromSensorReading(uint16_t sensorReading);
extern bool resetInvalidADCFilterValues(config4 &page4);
using map_sampling_runtime_fn_t = bool (*)(const statuses &, const config2 &, map_algorithm_t &, map_adc_readings_t &);
extern map_sampling_runtime_fn_t getMapSamplingFunction(MAPSamplingMethod mapSample);
extern bool instantaneousMAPReadingRuntime(const statuses &, const config2 &, map_algorithm_t &, map_adc_readings_t &);
extern bool cycleAverageMAPReadingRuntime(const statuses &, const config2 &, map_algorithm_t &, map_adc_readings_t &);
extern bool cycleMinimumMAPReadingRuntime(const statuses &, const config2 &, map_algorithm_t &, map_adc_readings_t &);
extern bool eventAverageMAPReadingRuntime(const statuses &, const config2 &, map_algorithm_t &, map_adc_readings_t &);

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

// ========================================== MAP / Baro Conversion Helpers ==========================================

static config2 calibrated_pressure_page(void) {
    config2 page2 {};
    page2.mapMin = -20;
    page2.mapMax = 300;
    page2.EMAPMin = -10;
    page2.EMAPMax = 400;
    page2.baroMin = 60;
    page2.baroMax = 120;
    return page2;
}

static void test_mapADCToMAP_clamps_negative_mapped_values_to_zero(void) {
    TEST_ASSERT_EQUAL_UINT16(0U, mapADCToMAP(0U, -20, 300));
    TEST_ASSERT_EQUAL_UINT16(0U, mapADCToMAP(32U, -20, 300));
}

static void test_mapADCToMAP_maps_midrange_values(void) {
    TEST_ASSERT_EQUAL_UINT16(140U, mapADCToMAP(512U, -20, 300));
    TEST_ASSERT_EQUAL_UINT16(299U, mapADCToMAP(1023U, -20, 300));
}

static void test_setMAPValuesFromReadings_updates_map_and_emap_when_enabled(void) {
    statuses current {};
    config2 page2 = calibrated_pressure_page();
    const map_adc_readings_t readings {
        .mapADC = 512U,
        .emapADC = 768U
    };

    setMAPValuesFromReadings(readings, page2, true, current);

    TEST_ASSERT_EQUAL_UINT16(140U, current.MAP);
    TEST_ASSERT_EQUAL_UINT16(297U, current.EMAP);
}

static void test_setMAPValuesFromReadings_leaves_emap_untouched_when_disabled(void) {
    statuses current {};
    config2 page2 = calibrated_pressure_page();
    const map_adc_readings_t readings {
        .mapADC = 640U,
        .emapADC = UINT16_MAX
    };
    current.EMAP = 222U;

    setMAPValuesFromReadings(readings, page2, false, current);

    TEST_ASSERT_EQUAL_UINT16(180U, current.MAP);
    TEST_ASSERT_EQUAL_UINT16(222U, current.EMAP);
}

static void test_setBaroFromSensorReading_updates_baro_and_adc(void) {
    configPage2 = calibrated_pressure_page();
    currentStatus.baro = 0U;
    currentStatus.baroADC = 0U;

    setBaroFromSensorReading(512U);

    TEST_ASSERT_EQUAL_UINT16(512U, currentStatus.baroADC);
    TEST_ASSERT_EQUAL_UINT8(90U, currentStatus.baro);
}

static void test_setBaroFromSensorReading_clamps_negative_mapped_baro_to_zero(void) {
    configPage2 = calibrated_pressure_page();
    configPage2.baroMin = -20;
    configPage2.baroMax = 100;
    currentStatus.baro = 55U;
    currentStatus.baroADC = 100U;

    setBaroFromSensorReading(0U);

    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.baroADC);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.baro);
}

static void test_resetInvalidADCFilterValues_resets_invalid_filter_values_to_defaults(void) {
    configPage4.ADCFILTER_TPS = 255U;
    configPage4.ADCFILTER_CLT = 255U;
    configPage4.ADCFILTER_IAT = 255U;
    configPage4.ADCFILTER_O2 = 255U;
    configPage4.ADCFILTER_BAT = 255U;
    configPage4.ADCFILTER_MAP = 255U;
    configPage4.ADCFILTER_BARO = 255U;
    configPage4.FILTER_FLEX = 255U;

    TEST_ASSERT_TRUE(resetInvalidADCFilterValues(configPage4));

    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_TPS_DEFAULT, configPage4.ADCFILTER_TPS);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_CLT_DEFAULT, configPage4.ADCFILTER_CLT);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_IAT_DEFAULT, configPage4.ADCFILTER_IAT);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_O2_DEFAULT, configPage4.ADCFILTER_O2);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_BAT_DEFAULT, configPage4.ADCFILTER_BAT);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_MAP_DEFAULT, configPage4.ADCFILTER_MAP);
    TEST_ASSERT_EQUAL_UINT8(ADCFILTER_BARO_DEFAULT, configPage4.ADCFILTER_BARO);
    TEST_ASSERT_EQUAL_UINT8(FILTER_FLEX_DEFAULT, configPage4.FILTER_FLEX);
}

static void test_getMapSamplingFunction_matches_runtime_selection(void) {
    TEST_ASSERT_EQUAL_PTR(instantaneousMAPReadingRuntime, getMapSamplingFunction(MAPSamplingInstantaneous));
    TEST_ASSERT_EQUAL_PTR(cycleAverageMAPReadingRuntime, getMapSamplingFunction(MAPSamplingCycleAverage));
    TEST_ASSERT_EQUAL_PTR(cycleMinimumMAPReadingRuntime, getMapSamplingFunction(MAPSamplingCycleMinimum));
    TEST_ASSERT_EQUAL_PTR(eventAverageMAPReadingRuntime, getMapSamplingFunction(MAPSamplingIgnitionEventAverage));
    TEST_ASSERT_EQUAL_PTR(instantaneousMAPReadingRuntime, getMapSamplingFunction((MAPSamplingMethod)99));
}

static void test_initialiseMAPBaro_with_external_baro_seeds_valid_sensor_value(void) {
    configPage6.useExtBaro = 1U;
    configPage2.baroMin = 100;
    configPage2.baroMax = 100;
    currentStatus.baro = 0U;
    currentStatus.baroADC = 0U;
    pinBaro = A0;

    initialiseMAPBaro();

    TEST_ASSERT_TRUE(isValidBaro(currentStatus.baro));
    TEST_ASSERT_EQUAL_UINT8(100U, currentStatus.baro);
}

static void test_initialiseMAPBaro_without_external_baro_keeps_valid_stored_baro_seed(void) {
    configPage6.useExtBaro = 0U;
    configPage2.mapMin = 0;
    configPage2.mapMax = 0;
    currentStatus.baro = 0U;
    pinMAP = A0;
    storeLastBaro(102U);

    initialiseMAPBaro();

    TEST_ASSERT_TRUE(isValidBaro(currentStatus.baro));
    TEST_ASSERT_EQUAL_UINT8(102U, currentStatus.baro);
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

    // MAP / baro conversion helper tests
    RUN_TEST(test_mapADCToMAP_clamps_negative_mapped_values_to_zero);
    RUN_TEST(test_mapADCToMAP_maps_midrange_values);
    RUN_TEST(test_setMAPValuesFromReadings_updates_map_and_emap_when_enabled);
    RUN_TEST(test_setMAPValuesFromReadings_leaves_emap_untouched_when_disabled);
    RUN_TEST(test_setBaroFromSensorReading_updates_baro_and_adc);
    RUN_TEST(test_setBaroFromSensorReading_clamps_negative_mapped_baro_to_zero);
    RUN_TEST(test_resetInvalidADCFilterValues_resets_invalid_filter_values_to_defaults);
    RUN_TEST(test_getMapSamplingFunction_matches_runtime_selection);
    RUN_TEST(test_initialiseMAPBaro_with_external_baro_seeds_valid_sensor_value);
    RUN_TEST(test_initialiseMAPBaro_without_external_baro_keeps_valid_stored_baro_seed);

    // fastMap10Bit edge case tests
    RUN_TEST(test_fastMap10Bit_zero_range);
    RUN_TEST(test_fastMap10Bit_full_ADC_range);
    RUN_TEST(test_fastMap10Bit_negative_to_positive);
    RUN_TEST(test_fastMap10Bit_large_range);
    RUN_TEST(test_fastMap10Bit_precision_loss);
  }
}
