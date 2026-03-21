#include <unity.h>
#include "globals.h"
#include "corrections.h"
#include "engineProtection.h"
#include "speeduino.h"
#include "../test_utils.h"

extern void construct2dTables(void);

static void seed_active_afr_inputs(void)
{
    currentStatus.MAP = 220U;
    currentStatus.TPS = 60U;
    currentStatus.O2 = 160U;
    currentStatus.afrTarget = 100U;
}

static void setup_limiter_state_machine(void)
{
    construct2dTables();
    initialiseCorrections();

    currentStatus.status1 = 0U;
    currentStatus.status2 = 0U;
    currentStatus.status5 = 0U;
    currentStatus.engineProtectStatus = 0U;
    currentStatus.launchingHard = false;
    currentStatus.launchingSoft = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.clutchTrigger = true;
    currentStatus.previousClutchTrigger = true;
    currentStatus.clutchEngagedRPM = 0U;
    currentStatus.RPM = 0U;
    currentStatus.RPMdiv100 = 0U;
    currentStatus.TPS = 0U;
    currentStatus.vss = 0U;
    currentStatus.MAP = 0U;
    currentStatus.O2 = 0U;
    currentStatus.afrTarget = 0U;

    configPage6.engineProtectType = PROTECT_CUT_OFF;
    configPage6.boostCutEnabled = 0U;
    configPage6.egoType = EGO_TYPE_OFF;
    configPage9.afrProtectEnabled = 0U;
    configPage10.oilPressureProtEnbl = false;
    configPage10.oilPressureEnable = false;
    checkAFRLimit();

    configPage2.vssMode = 0U;
    configPage2.hardCutType = HARD_CUT_FULL;
    configPage4.HardRevLim = 70U;
    configPage4.SoftRevLim = 65U;
    configPage4.SoftLimMax = 2U;
    configPage4.engineProtectMaxRPM = 50U;
    configPage6.engineProtectType = PROTECT_CUT_BOTH;
    configPage6.launchEnabled = 1U;
    configPage6.flatSEnable = 1U;
    configPage6.flatSArm = 20U;
    configPage6.lnchHardLim = 40U;
    configPage6.egoType = EGO_TYPE_WIDE;
    configPage9.hardRevMode = HARD_REV_FIXED;
    configPage9.afrProtectEnabled = 1U;
    configPage9.afrProtectMinMAP = 100U;
    configPage9.afrProtectMinRPM = 30U;
    configPage9.afrProtectMinTPS = 50U;
    configPage9.afrProtectDeviation = 150U;
    configPage9.afrProtectCutTime = 1U;
    configPage9.afrProtectReactivationTPS = 20U;
    configPage10.lnchCtrlTPS = 80U;
    configPage10.lnchCtrlVss = 50U;

    softLimitTime = 0U;
}

static void test_launch_state_machine_transitions_back_to_base_limit(void)
{
    setup_limiter_state_machine();
    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) - 100U;
    currentStatus.RPM = (configPage6.lnchHardLim * 100U) + 200U;
    currentStatus.TPS = configPage10.lnchCtrlTPS + 1U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.launchingHard);
    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT16(configPage6.lnchHardLim * 100U, calculateMaxAllowedRPM());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HLAUNCH, currentStatus.status2);

    currentStatus.RPM = (configPage6.lnchHardLim * 100U) - 100U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT16(configPage4.HardRevLim * 100U, calculateMaxAllowedRPM());
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_HLAUNCH, currentStatus.status2);
}

static void test_flat_shift_state_machine_uses_latched_clutch_rpm_until_exit(void)
{
    setup_limiter_state_machine();
    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) + 300U;
    currentStatus.RPM = currentStatus.clutchEngagedRPM + 200U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_TRUE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT16(currentStatus.clutchEngagedRPM, calculateMaxAllowedRPM());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSH, currentStatus.status5);

    currentStatus.RPM = currentStatus.clutchEngagedRPM - 50U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT16(configPage4.HardRevLim * 100U, calculateMaxAllowedRPM());
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSH, currentStatus.status5);
}

static void test_combined_rpm_limit_prioritizes_protection_then_launch_then_flat_shift(void)
{
    setup_limiter_state_machine();
    currentStatus.RPM = 7200U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    seed_active_afr_inputs();

    TEST_ASSERT_EQUAL_UINT16(configPage4.HardRevLim * 100U, calculateMaxAllowedRPM());

    delay(120);
    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_EQUAL_UINT16(configPage4.engineProtectMaxRPM * 100U, calculateMaxAllowedRPM());

    currentStatus.launchingHard = true;
    TEST_ASSERT_EQUAL_UINT16(configPage6.lnchHardLim * 100U, calculateMaxAllowedRPM());

    currentStatus.flatShiftingHard = true;
    currentStatus.clutchEngagedRPM = 3500U;
    TEST_ASSERT_EQUAL_UINT16(3500U, calculateMaxAllowedRPM());
}

static void test_launch_state_machine_rolling_cut_uses_delta_threshold_but_keeps_base_limit_source(void)
{
    setup_limiter_state_machine();
    configPage2.hardCutType = HARD_CUT_ROLLING;
    configPage15.rollingProtRPMDelta[0] = -20;

    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) - 100U;
    currentStatus.TPS = configPage10.lnchCtrlTPS + 1U;
    currentStatus.RPM = (configPage6.lnchHardLim * 100U) - 150U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HLAUNCH, currentStatus.status2);
    TEST_ASSERT_EQUAL_UINT16(configPage6.lnchHardLim * 100U, calculateMaxAllowedRPM());

    currentStatus.RPM = (configPage6.lnchHardLim * 100U) - 250U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_HLAUNCH, currentStatus.status2);
}

static void test_flat_shift_state_machine_rolling_cut_uses_delta_threshold_but_keeps_latched_limit_source(void)
{
    setup_limiter_state_machine();
    configPage2.hardCutType = HARD_CUT_ROLLING;
    configPage15.rollingProtRPMDelta[0] = -20;

    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) + 300U;
    currentStatus.RPM = currentStatus.clutchEngagedRPM - 150U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.flatShiftingHard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSH, currentStatus.status5);
    TEST_ASSERT_EQUAL_UINT16(currentStatus.clutchEngagedRPM, calculateMaxAllowedRPM());

    currentStatus.RPM = currentStatus.clutchEngagedRPM - 250U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSH, currentStatus.status5);
}

static void test_launch_state_machine_rolling_cut_respects_vss_gate_until_speed_drops(void)
{
    setup_limiter_state_machine();
    configPage2.hardCutType = HARD_CUT_ROLLING;
    configPage15.rollingProtRPMDelta[0] = -20;
    configPage2.vssMode = 1U;

    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) - 100U;
    currentStatus.TPS = configPage10.lnchCtrlTPS + 1U;
    currentStatus.RPM = (configPage6.lnchHardLim * 100U) - 150U;
    currentStatus.RPMdiv100 = currentStatus.RPM / 100U;
    currentStatus.vss = configPage10.lnchCtrlVss;

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_HLAUNCH, currentStatus.status2);
    TEST_ASSERT_EQUAL_UINT16(configPage4.HardRevLim * 100U, calculateMaxAllowedRPM());

    currentStatus.vss = configPage10.lnchCtrlVss - 1U;
    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HLAUNCH, currentStatus.status2);
    TEST_ASSERT_EQUAL_UINT16(configPage6.lnchHardLim * 100U, calculateMaxAllowedRPM());
}

static void test_afr_protection_state_machine_latches_until_reactivation_tps(void)
{
    setup_limiter_state_machine();
    seed_active_afr_inputs();
    currentStatus.RPMdiv100 = 40U;

    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    delay(120);
    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    currentStatus.O2 = 120U;
    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    currentStatus.TPS = configPage9.afrProtectReactivationTPS;
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);
}

static void test_afr_protection_target_table_mode_uses_target_plus_deviation_threshold(void)
{
    setup_limiter_state_machine();
    configPage9.afrProtectEnabled = 2U;
    configPage9.afrProtectDeviation = 15U;
    currentStatus.MAP = 220U;
    currentStatus.RPMdiv100 = 40U;
    currentStatus.TPS = 60U;
    currentStatus.afrTarget = 120U;
    currentStatus.O2 = 134U;

    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    delay(120);
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    currentStatus.O2 = 135U;
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    delay(120);
    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);
}

static void test_afr_protection_target_table_mode_stays_inactive_when_target_rises_above_current_o2(void)
{
    setup_limiter_state_machine();
    configPage9.afrProtectEnabled = 2U;
    configPage9.afrProtectDeviation = 15U;
    currentStatus.MAP = 220U;
    currentStatus.RPMdiv100 = 40U;
    currentStatus.TPS = 60U;
    currentStatus.afrTarget = 120U;
    currentStatus.O2 = 140U;

    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());

    currentStatus.afrTarget = 130U;
    delay(120);
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);
}

void test_limiter_state_machine(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST(test_launch_state_machine_transitions_back_to_base_limit);
        RUN_TEST(test_flat_shift_state_machine_uses_latched_clutch_rpm_until_exit);
        RUN_TEST(test_combined_rpm_limit_prioritizes_protection_then_launch_then_flat_shift);
        RUN_TEST(test_launch_state_machine_rolling_cut_uses_delta_threshold_but_keeps_base_limit_source);
        RUN_TEST(test_flat_shift_state_machine_rolling_cut_uses_delta_threshold_but_keeps_latched_limit_source);
        RUN_TEST(test_launch_state_machine_rolling_cut_respects_vss_gate_until_speed_drops);
        RUN_TEST(test_afr_protection_state_machine_latches_until_reactivation_tps);
        RUN_TEST(test_afr_protection_target_table_mode_uses_target_plus_deviation_threshold);
        RUN_TEST(test_afr_protection_target_table_mode_stays_inactive_when_target_rises_above_current_o2);
    }
}
