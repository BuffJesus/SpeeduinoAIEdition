#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Non360.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile unsigned long triggerSecFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_non360_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_NON360;
    configPage4.triggerTeeth = 6U;
    configPage4.TrigAngMul = 2U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;
    configPage4.triggerAngle = 10;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.StgCycles = 0U;
    configPage4.useResync = 0U;
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
    revolutionTime = 0U;
    revolutionOne = false;

    testClearTriggerStateOverrides();
    resetDecoder();
}

static void setup_non360_state_machine(void)
{
    reset_non360_runtime();
    triggerSetup_non360();
}

static void emit_non360_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_DualWheel();
}

static void emit_non360_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_DualWheel();
}

static void test_non360_setup_scales_tooth_angle_and_filter_seed(void)
{
    setup_non360_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(120U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(UINT8_MAX, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(555U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(833U, triggerSecFilterTime);
}

static void test_non360_getRPM_requires_sync_and_nonzero_tooth(void)
{
    setup_non360_state_machine();
    currentStatus.RPM = 321U;

    toothCurrentCount = 2U;
    TEST_ASSERT_EQUAL_UINT16(0U, getRPM_non360());

    currentStatus.hasSync = true;
    toothCurrentCount = 0U;
    TEST_ASSERT_EQUAL_UINT16(0U, getRPM_non360());
}

static void test_non360_secondary_sync_and_first_primary_wrap_follow_dualwheel_path(void)
{
    setup_non360_state_machine();

    emit_non360_secondary_gap(16000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);

    emit_non360_primary_gap(3000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(triggerFilterTime >= 700U);
    TEST_ASSERT_TRUE(triggerFilterTime < 850U);
}

static void test_non360_getCrankAngle_treats_secondary_tooth_as_last_primary_tooth(void)
{
    setup_non360_state_machine();
    SetRevolutionTime(60000U);
    toothLastToothTime = micros() - 150U;
    toothCurrentCount = 0U;

    TEST_ASSERT_INT16_WITHIN(3, 310, getCrankAngle_non360());
}

void testNon360()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_non360_setup_scales_tooth_angle_and_filter_seed);
        RUN_TEST_P(test_non360_getRPM_requires_sync_and_nonzero_tooth);
        RUN_TEST_P(test_non360_secondary_sync_and_first_primary_wrap_follow_dualwheel_path);
        RUN_TEST_P(test_non360_getCrankAngle_treats_secondary_tooth_as_last_primary_tooth);
    }
}
