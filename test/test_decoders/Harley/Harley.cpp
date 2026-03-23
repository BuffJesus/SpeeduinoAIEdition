#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Harley.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long curGap;
extern volatile unsigned long lastGap;
extern volatile unsigned long targetGap;
extern volatile unsigned long triggerFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_harley_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_HARLEY;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage4.crankRPM = 40U;
    configPage2.nCylinders = 2U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    curGap = 0U;
    lastGap = 0U;
    targetGap = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;

    testClearTriggerStateOverrides();
    resetDecoder();
}

static void setup_harley_state_machine(void)
{
    reset_harley_runtime();
    triggerSetup_Harley();
}

static void emit_harley_gap(unsigned long gapUs, bool primaryState)
{
    testSetPrimaryTriggerState(primaryState);
    delayMicroseconds(gapUs);
    triggerPri_Harley();
}

static void test_harley_setup_uses_zero_degree_seed_and_filter(void)
{
    setup_harley_state_machine();

    TEST_ASSERT_EQUAL_UINT16(0U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT32(1500U, triggerFilterTime);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

static void test_harley_first_long_gap_high_state_establishes_sync_on_tooth_one(void)
{
    setup_harley_state_machine();

    emit_harley_gap(3000U, true);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

static void test_harley_short_gap_after_sync_becomes_tooth_two(void)
{
    setup_harley_state_machine();

    emit_harley_gap(3000U, true);
    emit_harley_gap(2000U, true);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(157U, triggerToothAngle);
    TEST_ASSERT_UINT32_WITHIN(64U, 3000U, targetGap);
    TEST_ASSERT_EQUAL_UINT32(2U, currentStatus.startRevolutions);
}

static void test_harley_low_primary_state_drops_sync_and_clears_tooth(void)
{
    setup_harley_state_machine();

    emit_harley_gap(3000U, true);
    emit_harley_gap(3000U, false);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);
}

void testHarley()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_harley_setup_uses_zero_degree_seed_and_filter);
        RUN_TEST_P(test_harley_first_long_gap_high_state_establishes_sync_on_tooth_one);
        RUN_TEST_P(test_harley_short_gap_after_sync_becomes_tooth_two);
        RUN_TEST_P(test_harley_low_primary_state_drops_sync_and_clears_tooth);
    }
}
