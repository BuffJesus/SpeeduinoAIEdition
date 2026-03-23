#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "Gm7X.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long targetGap;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;

static void reset_gm7x_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_GM7X;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 6U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    targetGap = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_gm7x_state_machine(void)
{
    reset_gm7x_runtime();
    triggerSetup_GM7X();
}

static void emit_gm7x_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_GM7X();
}

static void test_gm7x_setup_uses_sixty_degree_teeth(void)
{
    setup_gm7x_state_machine();

    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_FALSE(currentStatus.hasSync);
}

static void test_gm7x_short_gap_establishes_sync_on_logical_tooth_three(void)
{
    setup_gm7x_state_machine();

    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);

    emit_gm7x_gap(900U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(32U, 1000U, targetGap);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(60U, triggerToothAngle);
}

static void test_gm7x_uniform_gaps_do_not_sync(void)
{
    setup_gm7x_state_machine();

    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(32U, 1000U, targetGap);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_gm7x_wraps_after_tooth_seven(void)
{
    setup_gm7x_state_machine();

    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);
    emit_gm7x_gap(900U);
    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);
    emit_gm7x_gap(2000U);

    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);

    emit_gm7x_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testGm7X()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_gm7x_setup_uses_sixty_degree_teeth);
        RUN_TEST_P(test_gm7x_short_gap_establishes_sync_on_logical_tooth_three);
        RUN_TEST_P(test_gm7x_uniform_gaps_do_not_sync);
        RUN_TEST_P(test_gm7x_wraps_after_tooth_seven);
    }
}
