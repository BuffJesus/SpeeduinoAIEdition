#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Daihatsu.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;

static void reset_daihatsu_runtime(uint8_t cylinders)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_DAIHATSU_PLUS1;
    configPage4.triggerAngle = 0;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage2.nCylinders = cylinders;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_daihatsu_state_machine(uint8_t cylinders)
{
    reset_daihatsu_runtime(cylinders);
    triggerSetup_Daihatsu();
    toothLastToothTime = micros();
}

static void emit_daihatsu_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Daihatsu();
}

static void test_daihatsu4_syncs_on_extra_tooth(void)
{
    setup_daihatsu_state_machine(4U);

    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(500U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_daihatsu4_wraps_after_tooth_five(void)
{
    setup_daihatsu_state_machine(4U);

    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(500U);

    emit_daihatsu_gap(2500U);
    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(5U, toothCurrentCount);

    emit_daihatsu_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

static void test_daihatsu3_syncs_on_extra_tooth(void)
{
    setup_daihatsu_state_machine(3U);

    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(400U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_daihatsu3_wraps_after_tooth_four(void)
{
    setup_daihatsu_state_machine(3U);

    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(2000U);
    emit_daihatsu_gap(400U);

    emit_daihatsu_gap(2800U);
    emit_daihatsu_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);

    emit_daihatsu_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testDaihatsu()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_daihatsu4_syncs_on_extra_tooth);
        RUN_TEST_P(test_daihatsu4_wraps_after_tooth_five);
        RUN_TEST_P(test_daihatsu3_syncs_on_extra_tooth);
        RUN_TEST_P(test_daihatsu3_wraps_after_tooth_four);
    }
}
