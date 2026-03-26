#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "ThirtySixMinus21.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_3621_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_36_2_1;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    revolutionOne = false;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_3621_state_machine(void)
{
    reset_3621_runtime();
    triggerSetup_ThirtySixMinus21();
    toothLastToothTime = micros();
}

static void emit_3621_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_ThirtySixMinus21();
}

void t3621(void)
{
    setup_3621_state_machine();
    emit_3621_gap(2000U);
    emit_3621_gap(2000U);
    emit_3621_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount); // Evidence-backed: single gap at position 18, so first tooth after it is position 19
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    // 15 more teeth to complete the 16-tooth run (count 19 is the 1st, 15 more = 16 total)
    for (uint8_t tooth = 0; tooth < 15U; ++tooth)
    {
        emit_3621_gap(2000U);
    }

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(34U, toothCurrentCount); // 19 + 15 = 34; double gap follows at positions 35-36
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
    TEST_ASSERT_FALSE(revolutionOne);

    emit_3621_gap(7000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    setup_3621_state_machine();
    emit_3621_gap(2000U);
    emit_3621_gap(2000U);
    emit_3621_gap(7000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    reset_3621_runtime();
}

void testThirtySixMinus21()
{
    RUN_TEST_P(t3621);
}
