#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Subaru67.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile unsigned long triggerSecFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_subaru67_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_SUBARU_67;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.ignCranklock = 0U;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    triggerFilterTime = 0U;
    triggerSecFilterTime = 0U;
    toothCurrentCount = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_subaru67_state_machine(void)
{
    reset_subaru67_runtime();
    triggerSetup_Subaru67();
    toothLastToothTime = micros();
    testSetLastSecondaryToothTime(micros());
}

static void emit_subaru67_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Subaru67();
}

static void emit_subaru67_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_Subaru67();
}

static void test_subaru67_setup_seeds_secondary_mode_and_tooth_one(void)
{
    setup_subaru67_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(2U, triggerToothAngle);
    TEST_ASSERT_TRUE(triggerFilterTime > 0U);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerSecFilterTime);
}

static void test_subaru67_three_cam_pulses_sync_next_primary_tooth_two(void)
{
    setup_subaru67_state_machine();

    emit_subaru67_secondary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());

    emit_subaru67_secondary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(2U, testGetSecondaryToothCount());
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    emit_subaru67_secondary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(3U, testGetSecondaryToothCount());

    emit_subaru67_primary_gap(1000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(93U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_subaru67_single_cam_pulse_at_wrong_tooth_realigns_tooth_five_without_sync(void)
{
    setup_subaru67_state_machine();

    emit_subaru67_secondary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());

    emit_subaru67_primary_gap(1000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
}

static void test_subaru67_synced_cycle_wraps_after_twelve_primary_teeth(void)
{
    setup_subaru67_state_machine();

    emit_subaru67_secondary_gap(1000U);
    emit_subaru67_secondary_gap(1000U);
    emit_subaru67_secondary_gap(1000U);
    emit_subaru67_primary_gap(1000U);

    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);

    for (uint8_t tooth = 0U; tooth < 10U; ++tooth)
    {
        emit_subaru67_primary_gap(1000U);
    }

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(12U, toothCurrentCount);

    emit_subaru67_primary_gap(1000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(55U, triggerToothAngle);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testSubaru67()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_subaru67_setup_seeds_secondary_mode_and_tooth_one);
        RUN_TEST_P(test_subaru67_three_cam_pulses_sync_next_primary_tooth_two);
        RUN_TEST_P(test_subaru67_single_cam_pulse_at_wrong_tooth_realigns_tooth_five_without_sync);
        RUN_TEST_P(test_subaru67_synced_cycle_wraps_after_twelve_primary_teeth);
    }
}
