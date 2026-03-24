#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "DRZ400.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile unsigned long triggerSecFilterTime;
extern volatile unsigned long curGap2;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_drz400_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_DRZ400;
    configPage4.triggerTeeth = 6U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.StgCycles = 0U;
    configPage4.useResync = 0U;
    configPage2.nCylinders = 1U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    triggerFilterTime = 0U;
    triggerSecFilterTime = 0U;
    curGap2 = 0U;
    toothCurrentCount = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    revolutionTime = 0U;
    revolutionOne = false;

    testClearTriggerStateOverrides();
    curGap2 = 0U;
    resetDecoder();
}

static void setup_drz400_state_machine(void)
{
    reset_drz400_runtime();
    triggerSetup_DRZ400();
}

static void cleanup_drz400_state_machine(void)
{
    testClearTriggerStateOverrides();
    resetDecoder();
}

static void emit_drz400_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_DualWheel();
}

static void emit_drz400_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_DRZ400();
}

static void test_drz400_setup_seeds_primary_and_secondary_filters(void)
{
    setup_drz400_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(UINT8_MAX, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(555U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(1666U, triggerSecFilterTime);

    cleanup_drz400_state_machine();
}

static void test_drz400_unsynced_secondary_pulse_establishes_sync_on_tooth_six(void)
{
    setup_drz400_state_machine();
    toothOneMinusOneTime = 1000U;
    toothOneTime = 9000U;

    emit_drz400_secondary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(4000U, triggerSecFilterTime);
    TEST_ASSERT_TRUE(toothLastToothTime > toothLastMinusOneToothTime);

    cleanup_drz400_state_machine();
}

static void test_drz400_synced_secondary_pulse_realigns_tooth_six_without_sync_loss_increment(void)
{
    setup_drz400_state_machine();
    currentStatus.hasSync = true;
    currentStatus.syncLossCounter = 2U;
    toothCurrentCount = 3U;
    toothOneMinusOneTime = 2000U;
    toothOneTime = 10000U;

    emit_drz400_secondary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(4000U, triggerSecFilterTime);

    cleanup_drz400_state_machine();
}

static void test_drz400_first_primary_after_secondary_reset_wraps_tooth_one(void)
{
    setup_drz400_state_machine();
    currentStatus.hasSync = true;
    toothCurrentCount = 6U;

    emit_drz400_primary_gap(3000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(triggerFilterTime >= 700U);
    TEST_ASSERT_TRUE(triggerFilterTime > 0U);

    cleanup_drz400_state_machine();
}

void testDRZ400()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_drz400_setup_seeds_primary_and_secondary_filters);
        RUN_TEST_P(test_drz400_unsynced_secondary_pulse_establishes_sync_on_tooth_six);
        RUN_TEST_P(test_drz400_synced_secondary_pulse_realigns_tooth_six_without_sync_loss_increment);
        RUN_TEST_P(test_drz400_first_primary_after_secondary_reset_wraps_tooth_one);
    }
}
