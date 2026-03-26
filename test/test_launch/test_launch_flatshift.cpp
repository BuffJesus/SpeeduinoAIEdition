#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "speeduino.h"

// pinLaunch is zero-initialised in globals.cpp (= 0).
// Under simavr with zero-state ATmega2560 all PORT/DDR/PIN registers start at 0x00,
// so digitalRead(0) returns 0 (LOW).  That makes clutch state determined by launchHiLo:
//   launchHiLo = 0 (active-low)  → clutchTrigger = !digitalRead(0) = 1  (clutch ENGAGED)
//   launchHiLo = 1 (active-high) → clutchTrigger =  digitalRead(0) = 0  (clutch RELEASED)
// By pre-setting currentStatus.clutchTrigger to match the expected pin output we prevent
// a spurious clutchEngagedRPM update (which only fires on a rising edge).

static void setup_launch_ctrl(void)
{
    currentStatus.RPM                   = 0U;
    currentStatus.TPS                   = 50U;
    currentStatus.vss                   = 0U;
    currentStatus.clutchTrigger         = true;  // pre-match active-low / pin-LOW state
    currentStatus.previousClutchTrigger = true;
    currentStatus.clutchEngagedRPM      = 0U;    // < flatSArm*100=6000 → launch window
    currentStatus.launchingHard         = false;
    currentStatus.flatShiftingHard      = false;
    currentStatus.status2               = 0U;
    currentStatus.status5               = 0U;

    configPage6.launchEnabled  = true;
    configPage6.launchHiLo     = 0U;    // active-low: pin LOW → clutch engaged
    configPage6.lnchHardLim    = 30U;   // 30 * 100 = 3000 RPM hard-launch limit
    configPage6.flatSEnable    = false;
    configPage6.flatSArm       = 60U;   // 60 * 100 = 6000 RPM flat-shift arm threshold

    configPage10.lnchCtrlTPS   = 0U;    // no TPS gate (TPS >= 0 always satisfied)
    configPage10.lnchCtrlVss   = 255U;  // no VSS gate (vss < 255 always satisfied)

    configPage2.vssMode        = 0U;              // no VSS mode → VSS check skipped
    configPage2.hardCutType    = HARD_CUT_FULL;   // no rolling-cut RPM delta
}

// RPM above hard-launch limit, clutch engaged → launchingHard and status bit asserted
static void test_launch_hard_activates_above_rpm_limit(void)
{
    setup_launch_ctrl();
    currentStatus.RPM = 4000U; // > lnchHardLim*100 = 3000

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.launchingHard);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HLAUNCH));
}

// RPM below hard-launch limit → launchingHard remains false
static void test_launch_not_active_below_rpm_limit(void)
{
    setup_launch_ctrl();
    currentStatus.RPM = 2000U; // < lnchHardLim*100 = 3000

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status2, BIT_STATUS2_HLAUNCH));
}

// Clutch released (active-high polarity, pin LOW → 0 = released) → no launch regardless of RPM
static void test_launch_not_active_when_clutch_released(void)
{
    setup_launch_ctrl();
    configPage6.launchHiLo          = 1U;    // active-high: pin LOW → clutch released
    currentStatus.clutchTrigger     = false; // pre-match to avoid edge detection
    currentStatus.previousClutchTrigger = false;
    currentStatus.RPM               = 4000U; // above limit, but clutch is out

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
}

// TPS below launch-control TPS gate → no launch even with clutch in and RPM over limit
static void test_launch_not_active_when_tps_below_gate(void)
{
    setup_launch_ctrl();
    currentStatus.RPM         = 4000U;
    currentStatus.TPS         = 20U;   // below gate
    configPage10.lnchCtrlTPS  = 50U;   // gate: TPS must be >= 50

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
}

// Flat shift: RPM above clutchEngagedRPM with clutch depressed at high speed → flatShiftingHard
static void test_flat_shift_activates_above_engaged_rpm(void)
{
    setup_launch_ctrl();
    configPage6.launchEnabled      = false;
    configPage6.flatSEnable        = true;
    currentStatus.clutchEngagedRPM = 7000U; // >= flatSArm*100=6000 → flat-shift window
    currentStatus.RPM              = 7500U; // > clutchEngagedRPM → limit firing

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT8(1U, BIT_CHECK(currentStatus.status5, BIT_STATUS5_FLATSH));
}

// Flat shift: RPM at or below clutchEngagedRPM → flatShiftingHard stays false
static void test_flat_shift_not_active_below_engaged_rpm(void)
{
    setup_launch_ctrl();
    configPage6.launchEnabled      = false;
    configPage6.flatSEnable        = true;
    currentStatus.clutchEngagedRPM = 7000U;
    currentStatus.RPM              = 6500U; // < clutchEngagedRPM

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_EQUAL_UINT8(0U, BIT_CHECK(currentStatus.status5, BIT_STATUS5_FLATSH));
}

void testLaunchAndFlatShift(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_launch_hard_activates_above_rpm_limit);
        RUN_TEST_P(test_launch_not_active_below_rpm_limit);
        RUN_TEST_P(test_launch_not_active_when_clutch_released);
        RUN_TEST_P(test_launch_not_active_when_tps_below_gate);
        RUN_TEST_P(test_flat_shift_activates_above_engaged_rpm);
        RUN_TEST_P(test_flat_shift_not_active_below_engaged_rpm);
    }
}
