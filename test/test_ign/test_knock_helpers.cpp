/**
 * @file test_knock_helpers.cpp
 * @brief Unit tests for knock protection helper functions
 *
 * Tests extracted TESTABLE_INLINE_STATIC helpers from corrections.cpp:
 * - knockActivationCount: Determines knock count threshold for retard activation
 * - calculateKnockRetard: Computes ignition retard based on knock count
 *
 * These are safety-critical functions that protect the engine from detonation damage.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "corrections.h"

// External test helpers exposed via TESTABLE_INLINE_STATIC
extern uint8_t knockActivationCount(const config10 &page10);
extern uint8_t calculateKnockRetard(uint8_t knockCount, const config10 &page10);

// ========================================== knockActivationCount Tests ==========================================

static void test_knockActivationCount_with_configured_value(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));

    // User configured knock_count = 3 (activate on 3rd knock)
    testConfig.knock_count = 3;
    TEST_ASSERT_EQUAL_UINT8(3, knockActivationCount(testConfig));

    // knock_count = 1 (activate immediately)
    testConfig.knock_count = 1;
    TEST_ASSERT_EQUAL_UINT8(1, knockActivationCount(testConfig));

    // knock_count = 5 (conservative, 3-bit field max is 7)
    testConfig.knock_count = 5;
    TEST_ASSERT_EQUAL_UINT8(5, knockActivationCount(testConfig));

    // knock_count = 7 (max value for 3-bit field)
    testConfig.knock_count = 7;
    TEST_ASSERT_EQUAL_UINT8(7, knockActivationCount(testConfig));
}

static void test_knockActivationCount_with_zero_defaults_to_one(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));

    // knock_count = 0 should default to 1 (prevent division by zero, activate immediately)
    testConfig.knock_count = 0;
    TEST_ASSERT_EQUAL_UINT8(1, knockActivationCount(testConfig));
}

// ========================================== calculateKnockRetard Tests ==========================================

static void test_calculateKnockRetard_below_activation_threshold(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 3;      // Activate on 3rd knock
    testConfig.knock_firstStep = 5;  // First retard: 5 degrees
    testConfig.knock_stepSize = 2;   // Each additional knock: +2 degrees

    // 0 knocks: no retard
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(0, testConfig));

    // 1 knock (below threshold of 3): still base retard
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(1, testConfig));

    // 2 knocks (below threshold of 3): still base retard
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(2, testConfig));
}

static void test_calculateKnockRetard_at_activation_threshold(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 3;      // Activate on 3rd knock
    testConfig.knock_firstStep = 5;  // First retard: 5 degrees
    testConfig.knock_stepSize = 2;   // Each additional knock: +2 degrees

    // 3 knocks (exactly at threshold): base retard
    // extraSteps = 3 - 3 = 0, retard = 5 + (0 * 2) = 5
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(3, testConfig));
}

static void test_calculateKnockRetard_above_activation_threshold(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 3;      // Activate on 3rd knock
    testConfig.knock_firstStep = 5;  // First retard: 5 degrees
    testConfig.knock_stepSize = 2;   // Each additional knock: +2 degrees

    // 4 knocks: extraSteps = 4 - 3 = 1, retard = 5 + (1 * 2) = 7
    TEST_ASSERT_EQUAL_UINT8(7, calculateKnockRetard(4, testConfig));

    // 5 knocks: extraSteps = 5 - 3 = 2, retard = 5 + (2 * 2) = 9
    TEST_ASSERT_EQUAL_UINT8(9, calculateKnockRetard(5, testConfig));

    // 10 knocks: extraSteps = 10 - 3 = 7, retard = 5 + (7 * 2) = 19
    TEST_ASSERT_EQUAL_UINT8(19, calculateKnockRetard(10, testConfig));
}

static void test_calculateKnockRetard_immediate_activation(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 1;      // Activate on 1st knock (immediate)
    testConfig.knock_firstStep = 3;  // First retard: 3 degrees
    testConfig.knock_stepSize = 1;   // Each additional knock: +1 degree

    // 1 knock: extraSteps = 1 - 1 = 0, retard = 3 + (0 * 1) = 3
    TEST_ASSERT_EQUAL_UINT8(3, calculateKnockRetard(1, testConfig));

    // 2 knocks: extraSteps = 2 - 1 = 1, retard = 3 + (1 * 1) = 4
    TEST_ASSERT_EQUAL_UINT8(4, calculateKnockRetard(2, testConfig));

    // 5 knocks: extraSteps = 5 - 1 = 4, retard = 3 + (4 * 1) = 7
    TEST_ASSERT_EQUAL_UINT8(7, calculateKnockRetard(5, testConfig));
}

static void test_calculateKnockRetard_zero_step_size(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 2;      // Activate on 2nd knock
    testConfig.knock_firstStep = 10; // First retard: 10 degrees
    testConfig.knock_stepSize = 0;   // No additional retard per knock

    // With step size = 0, retard stays at firstStep regardless of knock count
    TEST_ASSERT_EQUAL_UINT8(10, calculateKnockRetard(2, testConfig));
    TEST_ASSERT_EQUAL_UINT8(10, calculateKnockRetard(5, testConfig));
    TEST_ASSERT_EQUAL_UINT8(10, calculateKnockRetard(100, testConfig));
}

static void test_calculateKnockRetard_large_step_size(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 1;      // Activate immediately
    testConfig.knock_firstStep = 5;  // First retard: 5 degrees
    testConfig.knock_stepSize = 10;  // Large step: +10 degrees per knock

    // 1 knock: retard = 5 + (0 * 10) = 5
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(1, testConfig));

    // 2 knocks: retard = 5 + (1 * 10) = 15
    TEST_ASSERT_EQUAL_UINT8(15, calculateKnockRetard(2, testConfig));

    // 3 knocks: retard = 5 + (2 * 10) = 25
    TEST_ASSERT_EQUAL_UINT8(25, calculateKnockRetard(3, testConfig));

    // 5 knocks: retard = 5 + (4 * 10) = 45
    TEST_ASSERT_EQUAL_UINT8(45, calculateKnockRetard(5, testConfig));
}

static void test_calculateKnockRetard_overflow_protection(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 1;      // Activate immediately
    testConfig.knock_firstStep = 200; // High base retard
    testConfig.knock_stepSize = 50;  // Large steps

    // Validate saturation at 255 (overflow protection)
    // 2 knocks: retard = 200 + (1 * 50) = 250
    TEST_ASSERT_EQUAL_UINT8(250, calculateKnockRetard(2, testConfig));

    // 3 knocks: retard = 200 + (2 * 50) = 300, should saturate to 255
    TEST_ASSERT_EQUAL_UINT8(255, calculateKnockRetard(3, testConfig));

    // 4 knocks: retard = 200 + (3 * 50) = 350, should saturate to 255
    TEST_ASSERT_EQUAL_UINT8(255, calculateKnockRetard(4, testConfig));

    // 10 knocks: retard = 200 + (9 * 50) = 650, should saturate to 255
    TEST_ASSERT_EQUAL_UINT8(255, calculateKnockRetard(10, testConfig));
}

static void test_calculateKnockRetard_with_zero_knock_count_config(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    testConfig.knock_count = 0;      // Defaults to 1 via knockActivationCount
    testConfig.knock_firstStep = 5;  // First retard: 5 degrees
    testConfig.knock_stepSize = 2;   // Each additional knock: +2 degrees

    // knockActivationCount returns 1 when knock_count = 0
    // 1 knock: extraSteps = 1 - 1 = 0, retard = 5 + (0 * 2) = 5
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(1, testConfig));

    // 2 knocks: extraSteps = 2 - 1 = 1, retard = 5 + (1 * 2) = 7
    TEST_ASSERT_EQUAL_UINT8(7, calculateKnockRetard(2, testConfig));
}

static void test_calculateKnockRetard_realistic_scenario(void) {
    config10 testConfig;
    memset(&testConfig, 0, sizeof(testConfig));
    // Realistic values: activate on 2nd knock, 5° initial retard, 2° per additional knock
    testConfig.knock_count = 2;
    testConfig.knock_firstStep = 5;
    testConfig.knock_stepSize = 2;

    // Simulating knock event progression:
    // 1 knock: below threshold, retard = 5°
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(1, testConfig));

    // 2 knocks: at threshold, retard = 5°
    TEST_ASSERT_EQUAL_UINT8(5, calculateKnockRetard(2, testConfig));

    // 3 knocks: 1 extra, retard = 5 + (1 * 2) = 7°
    TEST_ASSERT_EQUAL_UINT8(7, calculateKnockRetard(3, testConfig));

    // 4 knocks: 2 extra, retard = 5 + (2 * 2) = 9°
    TEST_ASSERT_EQUAL_UINT8(9, calculateKnockRetard(4, testConfig));

    // 5 knocks: 3 extra, retard = 5 + (3 * 2) = 11°
    TEST_ASSERT_EQUAL_UINT8(11, calculateKnockRetard(5, testConfig));
}

// ========================================== Test Suite Entry Point ==========================================

void test_knock_helpers(void) {
  SET_UNITY_FILENAME() {
    // knockActivationCount tests
    RUN_TEST(test_knockActivationCount_with_configured_value);
    RUN_TEST(test_knockActivationCount_with_zero_defaults_to_one);

    // calculateKnockRetard tests
    RUN_TEST(test_calculateKnockRetard_below_activation_threshold);
    RUN_TEST(test_calculateKnockRetard_at_activation_threshold);
    RUN_TEST(test_calculateKnockRetard_above_activation_threshold);
    RUN_TEST(test_calculateKnockRetard_immediate_activation);
    RUN_TEST(test_calculateKnockRetard_zero_step_size);
    RUN_TEST(test_calculateKnockRetard_large_step_size);
    RUN_TEST(test_calculateKnockRetard_overflow_protection);
    RUN_TEST(test_calculateKnockRetard_with_zero_knock_count_config);
    RUN_TEST(test_calculateKnockRetard_realistic_scenario);
  }
}
