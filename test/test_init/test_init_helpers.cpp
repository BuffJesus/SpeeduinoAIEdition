/**
 * @file test_init_helpers.cpp
 * @brief Unit tests for init_helpers.h pure calculation functions
 */

#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>
#include "../test_utils.h"
#include "init_helpers.h"

// Test calculateReqFuelMicroseconds()
void test_calculateReqFuelMicroseconds_normal_range(void)
{
  // Normal range: 10ms to 25.5ms (100 to 255 in tenths)
  TEST_ASSERT_EQUAL_UINT16(10000, calculateReqFuelMicroseconds(100));  // 10.0ms
  TEST_ASSERT_EQUAL_UINT16(15500, calculateReqFuelMicroseconds(155));  // 15.5ms
  TEST_ASSERT_EQUAL_UINT16(25500, calculateReqFuelMicroseconds(255));  // 25.5ms
}

void test_calculateReqFuelMicroseconds_edge_cases(void)
{
  // Edge cases
  TEST_ASSERT_EQUAL_UINT16(0, calculateReqFuelMicroseconds(0));        // 0ms
  TEST_ASSERT_EQUAL_UINT16(100, calculateReqFuelMicroseconds(1));      // 0.1ms
  TEST_ASSERT_EQUAL_UINT16(65500, calculateReqFuelMicroseconds(655));  // 65.5ms (high value)
}

void test_calculateReqFuelMicroseconds_overflow_protection(void)
{
  // Test uint16_t overflow boundary (65535)
  // Max safe input: 655 (65.5ms → 65500µs)
  // Input 656 would overflow: 656 * 100 = 65600 > 65535
  TEST_ASSERT_EQUAL_UINT16(65500, calculateReqFuelMicroseconds(655));  // Max safe value
  // Overflow case: result wraps around (uint16_t behavior)
  TEST_ASSERT_EQUAL_UINT16(64, calculateReqFuelMicroseconds(656));     // 65600 % 65536 = 64
}

// Test calculateInjectorOpenTimeMicroseconds()
void test_calculateInjectorOpenTimeMicroseconds_normal_range(void)
{
  // Normal range: 0.5ms to 2.0ms (5 to 20 in tenths)
  TEST_ASSERT_EQUAL_UINT16(500, calculateInjectorOpenTimeMicroseconds(5));    // 0.5ms
  TEST_ASSERT_EQUAL_UINT16(1000, calculateInjectorOpenTimeMicroseconds(10));  // 1.0ms
  TEST_ASSERT_EQUAL_UINT16(1500, calculateInjectorOpenTimeMicroseconds(15));  // 1.5ms (common value)
  TEST_ASSERT_EQUAL_UINT16(2000, calculateInjectorOpenTimeMicroseconds(20));  // 2.0ms
}

void test_calculateInjectorOpenTimeMicroseconds_edge_cases(void)
{
  TEST_ASSERT_EQUAL_UINT16(0, calculateInjectorOpenTimeMicroseconds(0));      // 0ms
  TEST_ASSERT_EQUAL_UINT16(100, calculateInjectorOpenTimeMicroseconds(1));    // 0.1ms
  TEST_ASSERT_EQUAL_UINT16(3000, calculateInjectorOpenTimeMicroseconds(30));  // 3.0ms (high value)
}

// Test calculateStagedInjectorMultipliers()
void test_calculateStagedInjectorMultipliers_normal_range(void)
{
  uint16_t priMult, secMult;

  // Example from code comments:
  // Pri: 250cc, Sec: 500cc, Total: 750cc
  // Pri mult: (100 * 750) / 250 = 300%
  // Sec mult: (100 * 750) / 500 = 150%
  calculateStagedInjectorMultipliers(250, 500, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(300, priMult);
  TEST_ASSERT_EQUAL_UINT16(150, secMult);

  // Equal sized injectors (should both be 200%)
  // Pri: 500cc, Sec: 500cc, Total: 1000cc
  // Pri mult: (100 * 1000) / 500 = 200%
  // Sec mult: (100 * 1000) / 500 = 200%
  calculateStagedInjectorMultipliers(500, 500, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(200, priMult);
  TEST_ASSERT_EQUAL_UINT16(200, secMult);
}

void test_calculateStagedInjectorMultipliers_divide_by_zero(void)
{
  uint16_t priMult, secMult;

  // Pri injector = 0 (should default to 100%)
  calculateStagedInjectorMultipliers(0, 500, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(100, priMult);
  TEST_ASSERT_EQUAL_UINT16(100, secMult);

  // Sec injector = 0 (should default to 100%)
  calculateStagedInjectorMultipliers(500, 0, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(100, priMult);
  TEST_ASSERT_EQUAL_UINT16(100, secMult);

  // Both zero (should default to 100%)
  calculateStagedInjectorMultipliers(0, 0, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(100, priMult);
  TEST_ASSERT_EQUAL_UINT16(100, secMult);
}

void test_calculateStagedInjectorMultipliers_overflow_protection(void)
{
  uint16_t priMult, secMult;

  // Test overflow clamping to 65535
  // Pri: 100cc, Sec: 1000cc, Total: 1100cc
  // Pri mult: (100 * 1100) / 100 = 1100%
  // Sec mult: (100 * 1100) / 1000 = 110%
  calculateStagedInjectorMultipliers(100, 1000, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(1100, priMult);  // No clamping needed
  TEST_ASSERT_EQUAL_UINT16(110, secMult);

  // Extreme case: very small primary injector
  // Pri: 10cc, Sec: 1000cc, Total: 1010cc
  // Pri mult: (100 * 1010) / 10 = 10100%
  // Sec mult: (100 * 1010) / 1000 = 101%
  calculateStagedInjectorMultipliers(10, 1000, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(10100, priMult);  // No clamping needed
  TEST_ASSERT_EQUAL_UINT16(101, secMult);

  // Test actual overflow: Pri: 1cc, Sec: 1000cc, Total: 1001cc
  // Pri mult: (100 * 1001) / 1 = 100100% → clamped to 65535
  // Sec mult: (100 * 1001) / 1000 = 100%
  calculateStagedInjectorMultipliers(1, 1000, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(65535, priMult);  // Clamped
  TEST_ASSERT_EQUAL_UINT16(100, secMult);
}

void test_calculateStagedInjectorMultipliers_boundary_values(void)
{
  uint16_t priMult, secMult;

  // Test boundary at 655% result (uint16_t can hold up to 65535%)
  // Pri: 100cc, Sec: 555cc, Total: 655cc
  // Pri mult: (100 * 655) / 100 = 655%
  // Sec mult: (100 * 655) / 555 = 118%
  calculateStagedInjectorMultipliers(100, 555, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(655, priMult);
  TEST_ASSERT_EQUAL_UINT16(118, secMult);

  // Test at uint16_t boundary: Pri: 100cc, Sec: 6553500cc, Total: 6553600cc
  // Pri mult: (100 * 6553600) / 100 = 6553600% → clamped to 65535
  // This is an unrealistic configuration but tests overflow protection
  calculateStagedInjectorMultipliers(100, 6553500, priMult, secMult);
  TEST_ASSERT_EQUAL_UINT16(65535, priMult);  // Clamped
  TEST_ASSERT_EQUAL_UINT16(100, secMult);
}

// Note: int main() here overrides the Arduino framework entry point, intentionally
// keeping only this file's tests in scope to stay within ATmega2560 SRAM limits.
// The other test_init functions (testFuelScheduleInit etc.) exceed 8192 bytes when
// combined and are excluded from the simavr suite.
int main(void)
{
  UNITY_BEGIN();

  SET_UNITY_FILENAME() {
    RUN_TEST_P(test_calculateReqFuelMicroseconds_normal_range);
    RUN_TEST_P(test_calculateReqFuelMicroseconds_edge_cases);
    RUN_TEST_P(test_calculateReqFuelMicroseconds_overflow_protection);

    RUN_TEST_P(test_calculateInjectorOpenTimeMicroseconds_normal_range);
    RUN_TEST_P(test_calculateInjectorOpenTimeMicroseconds_edge_cases);

    RUN_TEST_P(test_calculateStagedInjectorMultipliers_normal_range);
    RUN_TEST_P(test_calculateStagedInjectorMultipliers_divide_by_zero);
    RUN_TEST_P(test_calculateStagedInjectorMultipliers_overflow_protection);
    RUN_TEST_P(test_calculateStagedInjectorMultipliers_boundary_values);
  }

  UNITY_END();

#if defined(SIMULATOR)
  cli();
  sleep_enable();
  sleep_cpu();
#endif

  return 0;
}
