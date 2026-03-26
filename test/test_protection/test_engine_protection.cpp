#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "engineProtection.h"

extern uint8_t softLimitTime;

// ---------------------------------------------------------------------------
// protectionTimerElapsed — pure helper, no globals
// ---------------------------------------------------------------------------

static void test_protection_timer_not_yet_elapsed(void)
{
    // elapsed = 5, delay = 10 → not yet
    TEST_ASSERT_FALSE(protectionTimerElapsed(55U, 50U, 10U));
}

static void test_protection_timer_just_elapsed(void)
{
    // elapsed = exactly delay → elapsed
    TEST_ASSERT_TRUE(protectionTimerElapsed(60U, 50U, 10U));
}

static void test_protection_timer_well_past(void)
{
    // elapsed = 30 > delay = 10
    TEST_ASSERT_TRUE(protectionTimerElapsed(80U, 50U, 10U));
}

static void test_protection_timer_uint8_wraparound(void)
{
    // currentTime=5, startTime=250, delay=10
    // (uint8_t)(5 - 250) = (uint8_t)(-245) = 11 >= 10 → elapsed
    TEST_ASSERT_TRUE(protectionTimerElapsed(5U, 250U, 10U));
}

// ---------------------------------------------------------------------------
// checkRevLimit — fixed-mode hard rev limiter
// ---------------------------------------------------------------------------

static void setup_rev_limit(void)
{
    currentStatus.RPMdiv100         = 0U;
    currentStatus.coolant           = 0;
    currentStatus.engineProtectStatus = 0U;
    currentStatus.status2           = 0U;

    configPage6.engineProtectType   = PROTECT_CUT_IGN;   // any non-OFF type
    configPage9.hardRevMode         = HARD_REV_FIXED;
    configPage4.HardRevLim          = 60U;  // 6000 RPM
    configPage4.SoftRevLim          = 55U;  // 5500 RPM
    configPage4.SoftLimMax          = 10U;  // 1.0 s soft window
    softLimitTime                   = 0U;   // soft window not yet exceeded
}

// RPM well below the hard limit → no protection bits set
static void test_rev_limit_below_hard_limit(void)
{
    setup_rev_limit();
    currentStatus.RPMdiv100 = 50U; // 5000 RPM < 6000 RPM limit

    checkRevLimit();

    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// RPM at the hard limit → RPM protect bit and legacy HRDLIM asserted
static void test_rev_limit_at_hard_limit(void)
{
    setup_rev_limit();
    currentStatus.RPMdiv100 = 60U; // exactly at HardRevLim

    checkRevLimit();

    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// engineProtectType == PROTECT_CUT_OFF → rev limiting disabled, no bits even above limit
static void test_rev_limit_protect_cut_off_no_action(void)
{
    setup_rev_limit();
    configPage6.engineProtectType   = PROTECT_CUT_OFF;
    currentStatus.RPMdiv100         = 70U; // above HardRevLim=60

    checkRevLimit();

    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// Soft window exceeded and RPM at soft limit → hard cut bits fire (2-stage limiter)
static void test_rev_limit_soft_window_exceeded_triggers_hard_cut(void)
{
    setup_rev_limit();
    softLimitTime           = 15U;  // > SoftLimMax=10 → window expired
    currentStatus.RPMdiv100 = 55U;  // at SoftRevLim; condition: softLimitTime>SoftLimMax && RPM>=SoftRevLim

    checkRevLimit();

    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// ---------------------------------------------------------------------------
// checkBoostLimit — MAP-based boost cut with three cut-type modes
// ---------------------------------------------------------------------------

static void setup_boost_limit(void)
{
    currentStatus.MAP               = 0U;
    currentStatus.engineProtectStatus = 0U;
    currentStatus.status1           = 0U;
    currentStatus.status2           = 0U;

    configPage6.engineProtectType   = PROTECT_CUT_IGN;
    configPage6.boostCutEnabled     = 1U;
    configPage6.boostLimit          = 100U;  // limit = 100 * 2 = 200 kPa
}

// MAP below boost limit → no cut, no status bits set
static void test_boost_limit_below_threshold(void)
{
    setup_boost_limit();
    currentStatus.MAP = 180U; // < boostLimit*2=200

    byte result = checkBoostLimit();

    TEST_ASSERT_EQUAL_UINT8(0U, result);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_BOOSTCUT));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status1, BIT_STATUS1_BOOSTCUT));
}

// MAP above limit, ignition cut mode → spark cut bit in status2 only
static void test_boost_limit_ign_cut_sets_spark_bit(void)
{
    setup_boost_limit();
    currentStatus.MAP             = 210U; // > boostLimit*2=200
    configPage6.engineProtectType = PROTECT_CUT_IGN;

    byte result = checkBoostLimit();

    TEST_ASSERT_EQUAL_UINT8(1U, result);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_BOOSTCUT));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status1, BIT_STATUS1_BOOSTCUT));
}

// MAP above limit, fuel cut mode → fuel cut bit in status1 only
static void test_boost_limit_fuel_cut_sets_fuel_bit(void)
{
    setup_boost_limit();
    currentStatus.MAP             = 210U;
    configPage6.engineProtectType = PROTECT_CUT_FUEL;

    byte result = checkBoostLimit();

    TEST_ASSERT_EQUAL_UINT8(1U, result);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_BOOSTCUT));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status1, BIT_STATUS1_BOOSTCUT));
}

// MAP above limit, both-cut mode → both bits set simultaneously
static void test_boost_limit_both_cut_sets_both_bits(void)
{
    setup_boost_limit();
    currentStatus.MAP             = 210U;
    configPage6.engineProtectType = PROTECT_CUT_BOTH;

    byte result = checkBoostLimit();

    TEST_ASSERT_EQUAL_UINT8(1U, result);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_BOOSTCUT));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status1, BIT_STATUS1_BOOSTCUT));
}

void testEngineProtection(void)
{
    SET_UNITY_FILENAME() {
        // protectionTimerElapsed — pure helper
        RUN_TEST_P(test_protection_timer_not_yet_elapsed);
        RUN_TEST_P(test_protection_timer_just_elapsed);
        RUN_TEST_P(test_protection_timer_well_past);
        RUN_TEST_P(test_protection_timer_uint8_wraparound);

        // checkRevLimit — fixed mode
        RUN_TEST_P(test_rev_limit_below_hard_limit);
        RUN_TEST_P(test_rev_limit_at_hard_limit);
        RUN_TEST_P(test_rev_limit_protect_cut_off_no_action);
        RUN_TEST_P(test_rev_limit_soft_window_exceeded_triggers_hard_cut);

        // checkBoostLimit — cut type modes
        RUN_TEST_P(test_boost_limit_below_threshold);
        RUN_TEST_P(test_boost_limit_ign_cut_sets_spark_bit);
        RUN_TEST_P(test_boost_limit_fuel_cut_sets_fuel_bit);
        RUN_TEST_P(test_boost_limit_both_cut_sets_both_bits);
    }
}
