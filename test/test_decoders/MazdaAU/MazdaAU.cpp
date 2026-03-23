#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "MazdaAU.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile unsigned long lastGap;
extern volatile unsigned long targetGap;

static void reset_mazdaau_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_MAZDA_AU;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.useResync = 0U;
    configPage4.ignCranklock = 0U;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    lastGap = 0U;
    targetGap = 0U;
    resetDecoder();
}

static void setup_mazdaau_state_machine(void)
{
    reset_mazdaau_runtime();
    triggerSetup_MazdaAU();
    toothLastToothTime = micros();
    testSetLastSecondaryToothTime(micros());
}

static void emit_mazdaau_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_MazdaAU();
}

static void emit_mazdaau_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_MazdaAU();
}

static void test_mazdaau_third_cam_tooth_establishes_sync(void)
{
    setup_mazdaau_state_machine();

    emit_mazdaau_secondary_gap(4000U);
    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());

    emit_mazdaau_secondary_gap(4000U);
    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(2U, testGetSecondaryToothCount());

    emit_mazdaau_secondary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(3U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_mazdaau_short_second_cam_gap_marks_close_pair_without_sync(void)
{
    setup_mazdaau_state_machine();

    emit_mazdaau_secondary_gap(4000U);
    emit_mazdaau_secondary_gap(1000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(99U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(3U, testGetSecondaryToothCount());
    TEST_ASSERT_UINT32_WITHIN(200U, 2000U, targetGap);
}

static void test_mazdaau_primary_cycle_wraps_after_four_edges(void)
{
    setup_mazdaau_state_machine();

    emit_mazdaau_secondary_gap(4000U);
    emit_mazdaau_secondary_gap(4000U);
    emit_mazdaau_secondary_gap(4000U);

    emit_mazdaau_primary_gap(10000U);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(108U, triggerToothAngle);

    emit_mazdaau_primary_gap(10000U);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(72U, triggerToothAngle);

    emit_mazdaau_primary_gap(10000U);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(108U, triggerToothAngle);

    emit_mazdaau_primary_gap(10000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(72U, triggerToothAngle);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testMazdaAU()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_mazdaau_third_cam_tooth_establishes_sync);
        RUN_TEST_P(test_mazdaau_short_second_cam_gap_marks_close_pair_without_sync);
        RUN_TEST_P(test_mazdaau_primary_cycle_wraps_after_four_edges);
    }
}
