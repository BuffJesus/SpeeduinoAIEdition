#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "engineProtection.h"

extern uint8_t softLimitTime;

static const uint8_t COOL_RPM_LIMIT = 60U; // 6000 RPM via coolantProtectTable
static const uint8_t OIL_MIN_PSI    = 20U; // minimum oil pressure threshold

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

// ---------------------------------------------------------------------------
// checkRevLimit — coolant-mode (HARD_REV_COOLANT) table-driven RPM limit
// ---------------------------------------------------------------------------

static void setup_coolant_rev_limit(void)
{
    setup_rev_limit();  // resets bits, configPage6.engineProtectType=PROTECT_CUT_IGN
    configPage9.hardRevMode = HARD_REV_COOLANT;
    // Uniform 6-bin table: all temp bins = CALIBRATION_TEMPERATURE_OFFSET → limit = COOL_RPM_LIMIT
    for (uint8_t i = 0U; i < 6U; i++) {
        configPage9.coolantProtTemp[i] = CALIBRATION_TEMPERATURE_OFFSET;
        configPage9.coolantProtRPM[i]  = COOL_RPM_LIMIT;
    }
    construct2dTable(coolantProtectTable, 6U, configPage9.coolantProtRPM, configPage9.coolantProtTemp);
    currentStatus.coolant = 0;  // lookup key = 0 + CALIBRATION_TEMPERATURE_OFFSET → hits all bins
}

// RPM below the coolant-table limit → no protection bits set
static void test_rev_limit_coolant_mode_below_limit(void)
{
    setup_coolant_rev_limit();
    currentStatus.RPMdiv100 = 50U;  // 5000 RPM < COOL_RPM_LIMIT (6000 RPM)
    checkRevLimit();
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_COOLANT));
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// RPM above the coolant-table limit → RPM + COOLANT protect bits + legacy HRDLIM all asserted
static void test_rev_limit_coolant_mode_above_limit(void)
{
    setup_coolant_rev_limit();
    currentStatus.RPMdiv100 = 70U;  // 7000 RPM > COOL_RPM_LIMIT (6000 RPM)
    checkRevLimit();
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_COOLANT));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM));
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HRDLIM));
}

// ---------------------------------------------------------------------------
// checkOilPressureLimit — RPM-keyed oil pressure table with timer-based delay
// ---------------------------------------------------------------------------

static void setup_oil_protect(void)
{
    currentStatus.RPMdiv100           = 30U;
    currentStatus.engineProtectStatus = 0U;
    configPage6.engineProtectType     = PROTECT_CUT_IGN;
    configPage10.oilPressureProtEnbl  = 1U;
    configPage10.oilPressureEnable    = 1U;
    configPage10.oilPressureProtTime  = 0U;   // zero delay → timer expires immediately
    // Uniform 4-bin table: all RPM bins = 0, all min pressures = OIL_MIN_PSI
    for (uint8_t i = 0U; i < 4U; i++) {
        configPage10.oilPressureProtRPM[i]  = 0U;
        configPage10.oilPressureProtMins[i] = OIL_MIN_PSI;
    }
    construct2dTable(oilPressureProtectTable, 4U, configPage10.oilPressureProtMins, configPage10.oilPressureProtRPM);
    // Reset the internal oilProtectCountEnabled flag by calling with pressure above limit
    currentStatus.oilPressure = 255U;
    checkOilPressureLimit();
    currentStatus.oilPressure         = 0U;   // restore: below OIL_MIN_PSI for actual tests
    currentStatus.engineProtectStatus = 0U;
}

// engineProtectType == PROTECT_CUT_OFF → oil protection disabled, no bits set
static void test_oil_protect_cut_off_disables(void)
{
    setup_oil_protect();
    configPage6.engineProtectType = PROTECT_CUT_OFF;
    byte result = checkOilPressureLimit();
    TEST_ASSERT_EQUAL_UINT8(0U, result);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL));
}

// oilPressureProtEnbl == 0 → inner gate not entered, no cut even with pressure below limit
static void test_oil_protect_disabled_no_cut(void)
{
    setup_oil_protect();
    configPage10.oilPressureProtEnbl = 0U;
    byte result = checkOilPressureLimit();
    TEST_ASSERT_EQUAL_UINT8(0U, result);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL));
}

// Oil pressure above table limit → no cut, resetOilProtectionTimer() called
static void test_oil_protect_pressure_above_limit_no_cut(void)
{
    setup_oil_protect();
    currentStatus.oilPressure = 50U;  // 50 PSI > OIL_MIN_PSI (20 PSI)
    byte result = checkOilPressureLimit();
    TEST_ASSERT_EQUAL_UINT8(0U, result);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL));
}

// Pressure below limit + zero delay: protectionTimerElapsed(0,0,0)=true → cut fires immediately
static void test_oil_protect_zero_delay_fires_immediately(void)
{
    setup_oil_protect();
    // currentStatus.oilPressure = 0 < OIL_MIN_PSI=20; oilPressureProtTime = 0
    byte result = checkOilPressureLimit();
    TEST_ASSERT_EQUAL_UINT8(1U, result);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL));
}

// ENGINE_PROTECT_BIT_OIL pre-set → alreadyActive=1 → cut fires even with non-zero delay
static void test_oil_protect_already_active_bypasses_delay(void)
{
    setup_oil_protect();
    configPage10.oilPressureProtTime = 100U;  // non-zero delay
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL);  // pre-set: alreadyActive=1
    byte result = checkOilPressureLimit();
    TEST_ASSERT_EQUAL_UINT8(1U, result);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL));
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

        // checkRevLimit — coolant mode (table-driven RPM limit)
        RUN_TEST_P(test_rev_limit_coolant_mode_below_limit);
        RUN_TEST_P(test_rev_limit_coolant_mode_above_limit);

        // checkOilPressureLimit — timer-based oil pressure protection
        RUN_TEST_P(test_oil_protect_cut_off_disables);
        RUN_TEST_P(test_oil_protect_disabled_no_cut);
        RUN_TEST_P(test_oil_protect_pressure_above_limit_no_cut);
        RUN_TEST_P(test_oil_protect_zero_delay_fires_immediately);
        RUN_TEST_P(test_oil_protect_already_active_bypasses_delay);
    }
}
