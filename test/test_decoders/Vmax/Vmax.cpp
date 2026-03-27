#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "../decoder_test_utils.h"
#include "Vmax.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_vmax_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_VMAX;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    triggerFilterTime = 0U;
    toothCurrentCount = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;

    primaryTriggerEdge = true;
    resetDecoderTestTransientState();
}

static void setup_vmax_state_machine(void)
{
    reset_vmax_runtime();
    triggerSetup_Vmax();
    toothLastToothTime = micros();
    testSetPrimaryTriggerState(false);
}

static void cleanup_vmax_state_machine(void)
{
    resetDecoderTestTransientState();
}

static void emit_vmax_edge(unsigned long gapUs, bool primaryState)
{
    testSetPrimaryTriggerState(primaryState);
    delayMicroseconds(gapUs);
    triggerPri_Vmax();
}

static void test_vmax_setup_seeds_filter_and_zero_degree_start(void)
{
    setup_vmax_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT32(1500U, triggerFilterTime);

    cleanup_vmax_state_machine();
}

static void test_vmax_first_wide_lobe_establishes_sync_on_tooth_one(void)
{
    setup_vmax_state_machine();

    emit_vmax_edge(10000U, true);
    emit_vmax_edge(3000U, false);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(80U, 3000U, lastGap);

    emit_vmax_edge(10000U, true);

    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(70U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);

    cleanup_vmax_state_machine();
}

static void test_vmax_small_lobe_after_sync_advances_tooth_counter(void)
{
    setup_vmax_state_machine();

    emit_vmax_edge(10000U, true);
    emit_vmax_edge(3000U, false);
    emit_vmax_edge(10000U, true);
    emit_vmax_edge(1000U, false);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(96U, 1000U, lastGap);

    emit_vmax_edge(10000U, true);

    TEST_ASSERT_EQUAL_UINT16(2U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(40U, triggerToothAngle);

    cleanup_vmax_state_machine();
}

static void test_vmax_wide_lobe_at_tooth_six_wraps_cleanly_to_tooth_one(void)
{
    setup_vmax_state_machine();

    currentStatus.hasSync = true;
    toothCurrentCount = 6U;
    lastGap = 1000U;
    triggerFilterTime = 0U;

    emit_vmax_edge(10000U, true);
    TEST_ASSERT_EQUAL_UINT16(6U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(70U, triggerToothAngle);

    emit_vmax_edge(3000U, false);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(80U, 3000U, lastGap);

    emit_vmax_edge(10000U, true);

    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(70U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    cleanup_vmax_state_machine();
}

void testVmax()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_vmax_setup_seeds_filter_and_zero_degree_start);
        RUN_TEST_P(test_vmax_first_wide_lobe_establishes_sync_on_tooth_one);
        RUN_TEST_P(test_vmax_small_lobe_after_sync_advances_tooth_counter);
        RUN_TEST_P(test_vmax_wide_lobe_at_tooth_six_wraps_cleanly_to_tooth_one);
    }
}
