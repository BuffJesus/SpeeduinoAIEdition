#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Gm24X.h"

extern volatile unsigned long toothLastToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_gm24x_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_24X;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 8U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_gm24x_state_machine(void)
{
    reset_gm24x_runtime();
    triggerSetup_24X();
}

static void emit_gm24x_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_24X();
}

static void emit_gm24x_cam_pulse(void)
{
    triggerSec_24X();
}

static void test_gm24x_startup_sentinel_blocks_primary_only_sync(void)
{
    setup_gm24x_state_machine();

    TEST_ASSERT_EQUAL_UINT16(25U, toothCurrentCount);

    emit_gm24x_primary_gap(4000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(25U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_gm24x_cam_reset_primes_first_primary_tooth(void)
{
    setup_gm24x_state_machine();

    emit_gm24x_cam_pulse();
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);

    emit_gm24x_primary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(15U, triggerToothAngle);
    TEST_ASSERT_FALSE(revolutionOne);
}

static void test_gm24x_lookup_table_advances_current_tooth_angles(void)
{
    setup_gm24x_state_machine();

    emit_gm24x_cam_pulse();
    emit_gm24x_primary_gap(4000U);

    emit_gm24x_primary_gap(4000U);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(6U, triggerToothAngle);

    emit_gm24x_primary_gap(4000U);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(15U, triggerToothAngle);
}

static void test_gm24x_cam_reset_restarts_next_revolution(void)
{
    setup_gm24x_state_machine();

    emit_gm24x_cam_pulse();
    emit_gm24x_primary_gap(4000U);
    emit_gm24x_primary_gap(4000U);
    emit_gm24x_primary_gap(4000U);

    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    emit_gm24x_cam_pulse();
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);

    emit_gm24x_primary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
    TEST_ASSERT_FALSE(revolutionOne);
}

void testGm24X()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_gm24x_startup_sentinel_blocks_primary_only_sync);
        RUN_TEST_P(test_gm24x_cam_reset_primes_first_primary_tooth);
        RUN_TEST_P(test_gm24x_lookup_table_advances_current_tooth_angles);
        RUN_TEST_P(test_gm24x_cam_reset_restarts_next_revolution);
    }
}
