/**
 * @file test_protection_timer.cpp
 * @brief Unit tests for engine protection timer helper
 *
 * Tests the protectionTimerElapsed function from engineProtection.cpp:
 * - Handles uint8_t timer wraparound (0-255) correctly
 * - Determines if protection delay period has elapsed
 * - Safety-critical for oil pressure, AFR, and other timed protection cutoffs
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"

// External function from engineProtection.cpp
extern bool protectionTimerElapsed(uint8_t currentTime, uint8_t startTime, uint8_t delayTime);

// ========================================== protectionTimerElapsed Tests ==========================================

static void test_protectionTimerElapsed_delay_not_reached(void) {
    // Start at time 10, current time 15, delay 10 -> elapsed = 5 < 10
    TEST_ASSERT_FALSE(protectionTimerElapsed(15, 10, 10));

    // Start at time 0, current time 5, delay 10 -> elapsed = 5 < 10
    TEST_ASSERT_FALSE(protectionTimerElapsed(5, 0, 10));

    // Start at time 100, current time 109, delay 10 -> elapsed = 9 < 10
    TEST_ASSERT_FALSE(protectionTimerElapsed(109, 100, 10));
}

static void test_protectionTimerElapsed_delay_exactly_reached(void) {
    // Start at time 10, current time 20, delay 10 -> elapsed = 10 == 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(20, 10, 10));

    // Start at time 0, current time 10, delay 10 -> elapsed = 10 == 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(10, 0, 10));

    // Start at time 200, current time 210, delay 10 -> elapsed = 10 == 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(210, 200, 10));
}

static void test_protectionTimerElapsed_delay_exceeded(void) {
    // Start at time 10, current time 25, delay 10 -> elapsed = 15 > 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(25, 10, 10));

    // Start at time 0, current time 50, delay 10 -> elapsed = 50 > 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(50, 0, 10));

    // Start at time 100, current time 200, delay 10 -> elapsed = 100 > 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(200, 100, 10));
}

static void test_protectionTimerElapsed_zero_delay(void) {
    // With delay = 0, protection should activate immediately
    TEST_ASSERT_TRUE(protectionTimerElapsed(10, 10, 0));
    TEST_ASSERT_TRUE(protectionTimerElapsed(15, 10, 0));
    TEST_ASSERT_TRUE(protectionTimerElapsed(0, 0, 0));
}

static void test_protectionTimerElapsed_wraparound_basic(void) {
    // Start at time 250, current time 5, delay 10
    // Wraparound: elapsed = (5 - 250) = 11 via uint8_t wraparound
    // uint8_t(5 - 250) = uint8_t(-245) = 11
    TEST_ASSERT_TRUE(protectionTimerElapsed(5, 250, 10));

    // Start at time 250, current time 0, delay 10
    // Wraparound: elapsed = (0 - 250) = 6 via uint8_t wraparound
    // uint8_t(0 - 250) = uint8_t(-250) = 6
    TEST_ASSERT_FALSE(protectionTimerElapsed(0, 250, 10));

    // Start at time 255, current time 5, delay 10
    // Wraparound: elapsed = (5 - 255) = 6 via uint8_t wraparound
    // uint8_t(5 - 255) = uint8_t(-250) = 6
    TEST_ASSERT_FALSE(protectionTimerElapsed(5, 255, 10));

    // Start at time 255, current time 10, delay 10
    // Wraparound: elapsed = (10 - 255) = 11 via uint8_t wraparound
    // uint8_t(10 - 255) = uint8_t(-245) = 11
    TEST_ASSERT_TRUE(protectionTimerElapsed(10, 255, 10));
}

static void test_protectionTimerElapsed_wraparound_exactly_at_boundary(void) {
    // Start at time 250, current time 4, delay 10
    // Wraparound: elapsed = (4 - 250) = 10 via uint8_t wraparound
    // uint8_t(4 - 250) = uint8_t(-246) = 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(4, 250, 10));

    // Start at time 255, current time 9, delay 10
    // Wraparound: elapsed = (9 - 255) = 10 via uint8_t wraparound
    // uint8_t(9 - 255) = uint8_t(-246) = 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(9, 255, 10));
}

static void test_protectionTimerElapsed_large_delay(void) {
    // Start at time 10, current time 20, delay 200 -> elapsed = 10 < 200
    TEST_ASSERT_FALSE(protectionTimerElapsed(20, 10, 200));

    // Start at time 10, current time 210, delay 200 -> elapsed = 200 == 200
    TEST_ASSERT_TRUE(protectionTimerElapsed(210, 10, 200));

    // Start at time 10, current time 220, delay 200 -> elapsed = 210 > 200
    TEST_ASSERT_TRUE(protectionTimerElapsed(220, 10, 200));
}

static void test_protectionTimerElapsed_max_delay(void) {
    // With delay = 255 (max uint8_t), only elapsed >= 255 should return true
    TEST_ASSERT_FALSE(protectionTimerElapsed(254, 0, 255));
    TEST_ASSERT_TRUE(protectionTimerElapsed(255, 0, 255));

    // Wraparound scenario: start 10, current 9 (wrapped), delay 255
    // elapsed = uint8_t(9 - 10) = uint8_t(-1) = 255
    TEST_ASSERT_TRUE(protectionTimerElapsed(9, 10, 255));
}

static void test_protectionTimerElapsed_realistic_oil_protection(void) {
    // Realistic scenario: Oil pressure protection with 5 second delay
    // Timer increments every 100ms (div100(millis())), so 5s = 50 ticks
    uint8_t startTime = 100;
    uint8_t delayTicks = 50;

    // At 4.9 seconds (49 ticks): protection not active yet
    TEST_ASSERT_FALSE(protectionTimerElapsed(149, startTime, delayTicks));

    // At 5.0 seconds (50 ticks): protection activates
    TEST_ASSERT_TRUE(protectionTimerElapsed(150, startTime, delayTicks));

    // At 6.0 seconds (60 ticks): protection remains active
    TEST_ASSERT_TRUE(protectionTimerElapsed(160, startTime, delayTicks));
}

static void test_protectionTimerElapsed_realistic_afr_protection(void) {
    // Realistic scenario: AFR protection with 2 second delay
    // Timer uses loop cycles, typical delay might be 20 ticks
    uint8_t startTime = 50;
    uint8_t delayTicks = 20;

    // Before delay: protection not active
    TEST_ASSERT_FALSE(protectionTimerElapsed(60, startTime, delayTicks));
    TEST_ASSERT_FALSE(protectionTimerElapsed(69, startTime, delayTicks));

    // At delay boundary: protection activates
    TEST_ASSERT_TRUE(protectionTimerElapsed(70, startTime, delayTicks));

    // After delay: protection remains active
    TEST_ASSERT_TRUE(protectionTimerElapsed(80, startTime, delayTicks));
    TEST_ASSERT_TRUE(protectionTimerElapsed(100, startTime, delayTicks));
}

static void test_protectionTimerElapsed_edge_case_same_time(void) {
    // Current time equals start time: no time has elapsed
    TEST_ASSERT_TRUE(protectionTimerElapsed(100, 100, 0));   // delay = 0, immediate
    TEST_ASSERT_FALSE(protectionTimerElapsed(100, 100, 1));  // delay > 0, not elapsed
    TEST_ASSERT_FALSE(protectionTimerElapsed(100, 100, 10)); // delay > 0, not elapsed
}

static void test_protectionTimerElapsed_wraparound_with_realistic_delay(void) {
    // Start near end of uint8_t range with realistic 5-second (50 tick) delay
    uint8_t startTime = 230;
    uint8_t delayTicks = 50;

    // Current time 240: elapsed = 10 < 50
    TEST_ASSERT_FALSE(protectionTimerElapsed(240, startTime, delayTicks));

    // Current time 255: elapsed = 25 < 50
    TEST_ASSERT_FALSE(protectionTimerElapsed(255, startTime, delayTicks));

    // Current time 0 (wrapped): elapsed = uint8_t(0 - 230) = 26 < 50
    TEST_ASSERT_FALSE(protectionTimerElapsed(0, startTime, delayTicks));

    // Current time 23 (wrapped): elapsed = uint8_t(23 - 230) = 49 < 50
    TEST_ASSERT_FALSE(protectionTimerElapsed(23, startTime, delayTicks));

    // Current time 24 (wrapped): elapsed = uint8_t(24 - 230) = 50 == 50
    TEST_ASSERT_TRUE(protectionTimerElapsed(24, startTime, delayTicks));

    // Current time 30 (wrapped): elapsed = uint8_t(30 - 230) = 56 > 50
    TEST_ASSERT_TRUE(protectionTimerElapsed(30, startTime, delayTicks));
}

// ========================================== Test Suite Entry Point ==========================================

void test_protection_timer(void) {
  SET_UNITY_FILENAME() {
    // Basic delay timing tests
    RUN_TEST(test_protectionTimerElapsed_delay_not_reached);
    RUN_TEST(test_protectionTimerElapsed_delay_exactly_reached);
    RUN_TEST(test_protectionTimerElapsed_delay_exceeded);
    RUN_TEST(test_protectionTimerElapsed_zero_delay);

    // Wraparound handling tests
    RUN_TEST(test_protectionTimerElapsed_wraparound_basic);
    RUN_TEST(test_protectionTimerElapsed_wraparound_exactly_at_boundary);

    // Large delay tests
    RUN_TEST(test_protectionTimerElapsed_large_delay);
    RUN_TEST(test_protectionTimerElapsed_max_delay);

    // Realistic scenario tests
    RUN_TEST(test_protectionTimerElapsed_realistic_oil_protection);
    RUN_TEST(test_protectionTimerElapsed_realistic_afr_protection);

    // Edge case tests
    RUN_TEST(test_protectionTimerElapsed_edge_case_same_time);
    RUN_TEST(test_protectionTimerElapsed_wraparound_with_realistic_delay);
  }
}
