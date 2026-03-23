#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Jeep2000.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;

static void reset_jeep2000_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_JEEP2000;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_OFF;
    configPage2.nCylinders = 6U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_jeep2000_state_machine(void)
{
    reset_jeep2000_runtime();
    triggerSetup_Jeep2000();
}

static void emit_jeep2000_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Jeep2000();
}

static void emit_jeep2000_cam_pulse(void)
{
    triggerSec_Jeep2000();
}

static void test_jeep2000_startup_sentinel_blocks_primary_only_sync(void)
{
    setup_jeep2000_state_machine();

    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);

    emit_jeep2000_primary_gap(6000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_jeep2000_cam_pulse_primes_next_primary_tooth_one(void)
{
    setup_jeep2000_state_machine();

    emit_jeep2000_cam_pulse();
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);

    emit_jeep2000_primary_gap(5500U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

static void test_jeep2000_grouped_primary_sequence_tracks_20_and_60_degree_steps(void)
{
    setup_jeep2000_state_machine();

    emit_jeep2000_cam_pulse();
    emit_jeep2000_primary_gap(5500U);

    emit_jeep2000_primary_gap(5500U);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(20U, triggerToothAngle);

    emit_jeep2000_primary_gap(5500U);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(20U, triggerToothAngle);

    emit_jeep2000_primary_gap(16700U);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(20U, triggerToothAngle);

    emit_jeep2000_primary_gap(5500U);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
}

static void test_jeep2000_cam_reset_starts_next_revolution(void)
{
    setup_jeep2000_state_machine();

    emit_jeep2000_cam_pulse();
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(16700U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(16700U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(16700U);
    emit_jeep2000_primary_gap(5500U);
    emit_jeep2000_primary_gap(5500U);

    TEST_ASSERT_EQUAL_UINT16(12U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    emit_jeep2000_cam_pulse();
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);

    emit_jeep2000_primary_gap(6700U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
}

void testJeep2000()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_jeep2000_startup_sentinel_blocks_primary_only_sync);
        RUN_TEST_P(test_jeep2000_cam_pulse_primes_next_primary_tooth_one);
        RUN_TEST_P(test_jeep2000_grouped_primary_sequence_tracks_20_and_60_degree_steps);
        RUN_TEST_P(test_jeep2000_cam_reset_starts_next_revolution);
    }
}
