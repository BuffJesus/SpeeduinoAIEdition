/**
 * @file test_vss_filter.cpp
 * @brief Unit tests for VSS pulse noise filter (vssPulseIsFiltered)
 *
 * Validates the contact-bounce / electrical-noise rejection logic introduced
 * in vssPulse(). The helper vssPulseIsFiltered() is a pure function extracted
 * for testability; the ISR delegates to it.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "sensors.h"

extern bool vssPulseIsFiltered(uint32_t now, uint32_t lastPulse);

// ---------------------------------------------------------------------------
// Filter constant sanity
// ---------------------------------------------------------------------------

static void test_vss_filter_min_gap_is_positive(void)
{
  TEST_ASSERT_GREATER_THAN_UINT32(0UL, VSS_FILTER_MIN_GAP_US);
}

static void test_vss_filter_min_gap_allows_2000hz_pulses(void)
{
  // 2000 Hz → 500µs gap — must NOT be filtered at the boundary
  TEST_ASSERT_FALSE(vssPulseIsFiltered(1000UL, 1000UL - VSS_FILTER_MIN_GAP_US));
}

// ---------------------------------------------------------------------------
// Accept legitimate pulses (gap >= VSS_FILTER_MIN_GAP_US)
// ---------------------------------------------------------------------------

static void test_vss_filter_accepts_pulse_at_exact_threshold(void)
{
  // gap == VSS_FILTER_MIN_GAP_US → accept (not strictly less than)
  uint32_t lastPulse = 5000UL;
  uint32_t now       = lastPulse + VSS_FILTER_MIN_GAP_US;
  TEST_ASSERT_FALSE(vssPulseIsFiltered(now, lastPulse));
}

static void test_vss_filter_accepts_pulse_well_above_threshold(void)
{
  // A normal driving pulse — 10 ms gap (100 Hz), well above 500µs
  uint32_t lastPulse = 0UL;
  uint32_t now       = 10000UL;
  TEST_ASSERT_FALSE(vssPulseIsFiltered(now, lastPulse));
}

static void test_vss_filter_accepts_first_pulse_after_long_stop(void)
{
  // Car was stationary; last pulse was 2 seconds ago
  uint32_t lastPulse = 0UL;
  uint32_t now       = 2000000UL;
  TEST_ASSERT_FALSE(vssPulseIsFiltered(now, lastPulse));
}

// ---------------------------------------------------------------------------
// Reject noise pulses (gap < VSS_FILTER_MIN_GAP_US)
// ---------------------------------------------------------------------------

static void test_vss_filter_rejects_pulse_just_below_threshold(void)
{
  uint32_t lastPulse = 5000UL;
  uint32_t now       = lastPulse + VSS_FILTER_MIN_GAP_US - 1UL;
  TEST_ASSERT_TRUE(vssPulseIsFiltered(now, lastPulse));
}

static void test_vss_filter_rejects_contact_bounce_at_10us(void)
{
  uint32_t lastPulse = 1000000UL;
  uint32_t now       = lastPulse + 10UL;
  TEST_ASSERT_TRUE(vssPulseIsFiltered(now, lastPulse));
}

static void test_vss_filter_rejects_zero_gap(void)
{
  // Same timestamp as last pulse — zero gap
  uint32_t lastPulse = 999999UL;
  TEST_ASSERT_TRUE(vssPulseIsFiltered(lastPulse, lastPulse));
}

// ---------------------------------------------------------------------------
// uint32_t overflow (micros() wraps after ~71 minutes)
// ---------------------------------------------------------------------------

static void test_vss_filter_handles_micros_overflow_accept(void)
{
  // last pulse just before overflow, now just after — gap = 600µs (accept)
  uint32_t lastPulse = 0xFFFFFFFFUL - 100UL;
  uint32_t now       = 499UL; // (now - lastPulse) with wrap = 100 + 499 = 599+1=600
  // (uint32_t)(now - lastPulse) = 0x00000000 + 499 - (0xFFFFFF.. - 100) wraps to 600
  TEST_ASSERT_FALSE(vssPulseIsFiltered(now, lastPulse));
}

static void test_vss_filter_handles_micros_overflow_reject(void)
{
  // gap of only 100µs across overflow boundary — reject
  uint32_t lastPulse = 0xFFFFFFFFUL - 50UL;
  uint32_t now       = 49UL; // wrapped gap = 50 + 49 + 1 = 100µs < 500µs
  TEST_ASSERT_TRUE(vssPulseIsFiltered(now, lastPulse));
}

// ---------------------------------------------------------------------------
// Runner
// ---------------------------------------------------------------------------

void test_vss_filter(void)
{
  RUN_TEST(test_vss_filter_min_gap_is_positive);
  RUN_TEST(test_vss_filter_min_gap_allows_2000hz_pulses);
  RUN_TEST(test_vss_filter_accepts_pulse_at_exact_threshold);
  RUN_TEST(test_vss_filter_accepts_pulse_well_above_threshold);
  RUN_TEST(test_vss_filter_accepts_first_pulse_after_long_stop);
  RUN_TEST(test_vss_filter_rejects_pulse_just_below_threshold);
  RUN_TEST(test_vss_filter_rejects_contact_bounce_at_10us);
  RUN_TEST(test_vss_filter_rejects_zero_gap);
  RUN_TEST(test_vss_filter_handles_micros_overflow_accept);
  RUN_TEST(test_vss_filter_handles_micros_overflow_reject);
}
