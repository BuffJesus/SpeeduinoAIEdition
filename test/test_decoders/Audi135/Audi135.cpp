#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Audi135.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long toothSystemLastToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_audi135_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_AUDI135;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.useResync = 0;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothSystemLastToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_audi135_state_machine(void)
{
    reset_audi135_runtime();
    triggerSetup_Audi135();
    toothLastToothTime = micros();
    toothSystemLastToothTime = toothLastToothTime;
}

static void emit_audi135_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Audi135();
}

static void test_audi135_cam_sync_primes_effective_tooth_counter(void)
{
    setup_audi135_state_machine();

    emit_audi135_primary_gap(740U);
    emit_audi135_primary_gap(740U);
    emit_audi135_primary_gap(740U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(UINT8_MAX, toothCurrentCount);

    triggerSec_Audi135();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);

    emit_audi135_primary_gap(740U);

    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);

    emit_audi135_primary_gap(740U);
    emit_audi135_primary_gap(740U);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    emit_audi135_primary_gap(740U);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
}

static void test_audi135_wraps_after_effective_tooth45(void)
{
    setup_audi135_state_machine();
    triggerSec_Audi135();

    toothCurrentCount = 45U;
    currentStatus.startRevolutions = 1U;
    revolutionOne = false;

    emit_audi135_primary_gap(740U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

static void test_audi135_resync_resets_effective_counter_until_next_counted_tooth(void)
{
    setup_audi135_state_machine();
    triggerSec_Audi135();
    emit_audi135_primary_gap(740U);

    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    configPage4.useResync = 1;
    triggerSec_Audi135();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);

    emit_audi135_primary_gap(740U);

    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
    TEST_ASSERT_FALSE(revolutionOne);
}

void testAudi135()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_audi135_cam_sync_primes_effective_tooth_counter);
        RUN_TEST_P(test_audi135_wraps_after_effective_tooth45);
        RUN_TEST_P(test_audi135_resync_resets_effective_counter_until_next_counted_tooth);
    }
}
