#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "HondaJ32.h"
extern volatile unsigned long toothLastToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile unsigned long lastGap;

static void reset_hondaj32_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_HONDA_J32;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 6U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothCurrentCount = 0U;
    lastGap = 0U;
    resetDecoder();
}

static void setup_hondaj32_state_machine(void)
{
    reset_hondaj32_runtime();
    triggerSetup_HondaJ32();
    toothLastToothTime = micros();
}

static void emit_hondaj32_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_HondaJ32();
}

void test_hj32_sync(void)
{
    setup_hondaj32_state_machine();

    for (uint8_t tooth = 0; tooth < 15U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);

    setup_hondaj32_state_machine();

    for (uint8_t tooth = 0; tooth < 7U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(4000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

void test_hj32_wrap(void)
{
    setup_hondaj32_state_machine();

    for (uint8_t tooth = 0; tooth < 15U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(4000U);

    for (uint8_t tooth = 0; tooth < 6U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(2000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);

    setup_hondaj32_state_machine();

    for (uint8_t tooth = 0; tooth < 15U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(4000U);

    for (uint8_t tooth = 0; tooth < 6U; ++tooth)
    {
        emit_hondaj32_primary_gap(2000U);
    }
    emit_hondaj32_primary_gap(4000U);
    emit_hondaj32_primary_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, revolutionTime);
}

void testHondaJ32()
{
    RUN_TEST_P(test_hj32_sync);
    RUN_TEST_P(test_hj32_wrap);
}
