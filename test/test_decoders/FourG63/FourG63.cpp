#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "FourG63.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile unsigned long triggerSecFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;
extern volatile bool revolutionOne;
extern volatile unsigned long secondaryLastToothTime1;

static void reset_4g63_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 0U;
    configPage4.ignCranklock = 0U;
    configPage4.StgCycles = 0U;

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
    revolutionOne = false;
    secondaryLastToothTime1 = 0U;

    testClearTriggerStateOverrides();
    resetDecoder();
}

static void setup_4g63_state_machine(void)
{
    reset_4g63_runtime();
    triggerSetup_4G63();
    testSetPrimaryTriggerState(false);
    testSetSecondaryTriggerState(false);
}

static void cleanup_4g63_state_machine(void)
{
    testClearTriggerStateOverrides();
}

static void emit_4g63_primary_gap(unsigned long gapUs, bool primaryHigh, bool secondaryHigh)
{
    testSetPrimaryTriggerState(primaryHigh);
    testSetSecondaryTriggerState(secondaryHigh);
    delayMicroseconds(gapUs);
    triggerPri_4G63();
}

static void emit_4g63_secondary_gap(unsigned long gapUs, bool primaryHigh, bool secondaryHigh)
{
    testSetPrimaryTriggerState(primaryHigh);
    testSetSecondaryTriggerState(secondaryHigh);
    delayMicroseconds(gapUs);
    triggerSec_4G63();
}

static void establish_4g63_sync_candidate_tooth_five(void)
{
    emit_4g63_primary_gap(2000U, true, true);
    emit_4g63_primary_gap(2000U, false, true);
}

static void test_4g63_setup_seeds_unsynced_state(void)
{
    setup_4g63_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(99U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(180U, triggerToothAngle);
    TEST_ASSERT_TRUE(triggerFilterTime > 0U);

    cleanup_4g63_state_machine();
}

static void test_4g63_primary_high_with_cam_high_marks_first_revolution_candidate(void)
{
    setup_4g63_state_machine();

    emit_4g63_primary_gap(2000U, true, true);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    cleanup_4g63_state_machine();
}

static void test_4g63_primary_low_with_cam_high_lands_candidate_tooth_five(void)
{
    setup_4g63_state_machine();

    establish_4g63_sync_candidate_tooth_five();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    cleanup_4g63_state_machine();
}

static void test_4g63_secondary_low_while_primary_low_establishes_sync_on_tooth_five(void)
{
    setup_4g63_state_machine();

    establish_4g63_sync_candidate_tooth_five();
    emit_4g63_secondary_gap(2000U, false, false);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    cleanup_4g63_state_machine();
}

static void test_4g63_synced_primary_progression_advances_from_tooth_five(void)
{
    setup_4g63_state_machine();

    establish_4g63_sync_candidate_tooth_five();
    emit_4g63_secondary_gap(2000U, false, false);
    emit_4g63_primary_gap(2000U, true, true);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(110U, triggerToothAngle);

    cleanup_4g63_state_machine();
}

void testFourG63()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_4g63_setup_seeds_unsynced_state);
        RUN_TEST_P(test_4g63_primary_high_with_cam_high_marks_first_revolution_candidate);
        RUN_TEST_P(test_4g63_primary_low_with_cam_high_lands_candidate_tooth_five);
        RUN_TEST_P(test_4g63_secondary_low_while_primary_low_establishes_sync_on_tooth_five);
        RUN_TEST_P(test_4g63_synced_primary_progression_advances_from_tooth_five);
    }
}
