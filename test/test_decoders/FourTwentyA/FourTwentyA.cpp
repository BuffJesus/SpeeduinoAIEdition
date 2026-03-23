#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "FourTwentyA.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_420a_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_420A;
    configPage4.TrigSpeed = CAM_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    triggerFilterTime = 0U;
    toothCurrentCount = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;

    testClearTriggerStateOverrides();
    resetDecoder();
}

static void setup_420a_state_machine(void)
{
    reset_420a_runtime();
    triggerSetup_420a();
    toothLastToothTime = micros();
    testSetPrimaryTriggerState(false);
}

static void cleanup_420a_state_machine(void)
{
    testClearTriggerStateOverrides();
}

static void emit_420a_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_420a();
}

static void emit_420a_secondary_falling(bool primaryHigh)
{
    testSetPrimaryTriggerState(primaryHigh);
    triggerSec_420a();
}

static void test_420a_setup_starts_on_tooth_one_without_sync(void)
{
    setup_420a_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(20U, triggerToothAngle);
    TEST_ASSERT_TRUE(triggerFilterTime > 0U);

    cleanup_420a_state_machine();
}

static void test_420a_secondary_falling_with_primary_high_syncs_tooth_thirteen(void)
{
    setup_420a_state_machine();

    emit_420a_secondary_falling(true);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);

    cleanup_420a_state_machine();
}

static void test_420a_secondary_falling_with_primary_low_syncs_tooth_five(void)
{
    setup_420a_state_machine();

    emit_420a_secondary_falling(false);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);

    cleanup_420a_state_machine();
}

static void test_420a_synced_secondary_high_realigns_wrong_tooth_and_counts_sync_loss(void)
{
    setup_420a_state_machine();

    currentStatus.hasSync = true;
    toothCurrentCount = 12U;

    emit_420a_secondary_falling(true);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);

    cleanup_420a_state_machine();
}

static void test_420a_wraps_after_tooth_sixteen(void)
{
    setup_420a_state_machine();

    currentStatus.hasSync = true;
    toothCurrentCount = 13U;

    emit_420a_primary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(14U, toothCurrentCount);

    emit_420a_primary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(15U, toothCurrentCount);

    emit_420a_primary_gap(1000U);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);

    emit_420a_primary_gap(1000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);

    cleanup_420a_state_machine();
}

void testFourTwentyA()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_420a_setup_starts_on_tooth_one_without_sync);
        RUN_TEST_P(test_420a_secondary_falling_with_primary_high_syncs_tooth_thirteen);
        RUN_TEST_P(test_420a_secondary_falling_with_primary_low_syncs_tooth_five);
        RUN_TEST_P(test_420a_synced_secondary_high_realigns_wrong_tooth_and_counts_sync_loss);
        RUN_TEST_P(test_420a_wraps_after_tooth_sixteen);
    }
}
