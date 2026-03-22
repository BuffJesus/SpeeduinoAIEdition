#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "ThirtySixMinus222.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_36222_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_36_2_2_2;
    configPage4.triggerAngle = 0;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_36222_state_machine(void)
{
    reset_36222_runtime();
    triggerSetup_ThirtySixMinus222();
    toothLastToothTime = micros();
}

static void emit_36222_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_ThirtySixMinus222();
}

static void test_36222_h4_double_gap_path_syncs_tooth19(void)
{
    setup_36222_state_machine();

    emit_36222_gap(2000U);
    emit_36222_gap(2000U);
    emit_36222_gap(6000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    emit_36222_gap(6000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));
}

static void test_36222_h4_single_gap_path_syncs_tooth35(void)
{
    setup_36222_state_machine();

    emit_36222_gap(2000U);
    emit_36222_gap(2000U);
    emit_36222_gap(6000U);
    emit_36222_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(35U, toothCurrentCount);
    TEST_ASSERT_TRUE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));
}

static void test_36222_h4_wrap_counts_revolution(void)
{
    setup_36222_state_machine();

    emit_36222_gap(2000U);
    emit_36222_gap(2000U);
    emit_36222_gap(6000U);
    emit_36222_gap(2000U);

    emit_36222_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(36U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);

    emit_36222_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testThirtySixMinus222()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_36222_h4_double_gap_path_syncs_tooth19);
        RUN_TEST_P(test_36222_h4_single_gap_path_syncs_tooth35);
        RUN_TEST_P(test_36222_h4_wrap_counts_revolution);
    }
}
