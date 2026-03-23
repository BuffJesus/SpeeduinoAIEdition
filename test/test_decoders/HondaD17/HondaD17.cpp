#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "HondaD17.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long lastGap;
extern volatile unsigned long targetGap;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_hondad17_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_HONDA_D17;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    lastGap = 0U;
    targetGap = 0U;
    triggerToothAngle = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_hondad17_state_machine(void)
{
    reset_hondad17_runtime();
    triggerSetup_HondaD17();
    toothLastToothTime = micros();
}

static void emit_hondad17_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_HondaD17();
}

static void test_hondad17_setup_uses_thirty_degree_teeth(void)
{
    setup_hondad17_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(30U, triggerToothAngle);
}

static void test_hondad17_short_gap_on_thirteenth_tooth_establishes_sync(void)
{
    setup_hondad17_state_machine();

    for (uint8_t tooth = 0U; tooth < 12U; ++tooth)
    {
        emit_hondad17_gap(10000U);
    }

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(12U, toothCurrentCount);

    emit_hondad17_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(64U, 5000U, targetGap);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_hondad17_uniform_gaps_do_not_sync(void)
{
    setup_hondad17_state_machine();

    for (uint8_t tooth = 0U; tooth < 13U; ++tooth)
    {
        emit_hondad17_gap(10000U);
    }

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(64U, 5000U, targetGap);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_hondad17_synced_cycle_wraps_on_next_reference_tooth(void)
{
    setup_hondad17_state_machine();

    for (uint8_t tooth = 0U; tooth < 12U; ++tooth)
    {
        emit_hondad17_gap(10000U);
    }
    emit_hondad17_gap(4000U);

    for (uint8_t tooth = 0U; tooth < 12U; ++tooth)
    {
        emit_hondad17_gap(10000U);
    }

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(12U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    emit_hondad17_gap(4000U);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);

    emit_hondad17_gap(10000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testHondaD17()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_hondad17_setup_uses_thirty_degree_teeth);
        RUN_TEST_P(test_hondad17_short_gap_on_thirteenth_tooth_establishes_sync);
        RUN_TEST_P(test_hondad17_uniform_gaps_do_not_sync);
        RUN_TEST_P(test_hondad17_synced_cycle_wraps_on_next_reference_tooth);
    }
}
