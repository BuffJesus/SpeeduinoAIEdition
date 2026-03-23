#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "BasicDistributor.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long triggerFilterTime;
extern volatile uint16_t toothCurrentCount;
extern volatile uint16_t triggerToothAngle;
extern uint16_t triggerActualTeeth;

static void reset_basic_distributor_runtime(uint8_t strokes)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    configPage4.TrigPattern = DECODER_BASIC_DISTRIBUTOR;
    configPage4.TrigSpeed = CAM_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.ignCranklock = false;
    configPage2.nCylinders = 4U;
    configPage2.strokes = strokes;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    triggerFilterTime = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoder();
}

static void setup_basic_distributor_state_machine(uint8_t strokes)
{
    reset_basic_distributor_runtime(strokes);
    triggerSetup_BasicDistributor();
    toothLastToothTime = micros();
}

static void emit_basic_distributor_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_BasicDistributor();
}

static void test_basic_distributor_setup_uses_stroke_dependent_tooth_angle(void)
{
    setup_basic_distributor_state_machine(FOUR_STROKE);

    TEST_ASSERT_EQUAL_UINT16(4U, triggerActualTeeth);
    TEST_ASSERT_EQUAL_UINT16(180U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);

    setup_basic_distributor_state_machine(TWO_STROKE);

    TEST_ASSERT_EQUAL_UINT16(4U, triggerActualTeeth);
    TEST_ASSERT_EQUAL_UINT16(90U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);
}

static void test_basic_distributor_first_pulse_establishes_sync_then_counts_tooth_two(void)
{
    setup_basic_distributor_state_machine(FOUR_STROKE);

    emit_basic_distributor_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);

    emit_basic_distributor_gap(2000U);

    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(32U, 500U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);
}

static void test_basic_distributor_wraps_on_fifth_pulse_for_four_cylinder_four_stroke(void)
{
    setup_basic_distributor_state_machine(FOUR_STROKE);

    emit_basic_distributor_gap(2000U);
    emit_basic_distributor_gap(2000U);
    emit_basic_distributor_gap(2000U);
    emit_basic_distributor_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);

    emit_basic_distributor_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(2U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneMinusOneTime);
}

static void test_basic_distributor_short_gap_is_filtered_after_sync(void)
{
    setup_basic_distributor_state_machine(FOUR_STROKE);

    emit_basic_distributor_gap(2000U);
    emit_basic_distributor_gap(2000U);
    const unsigned long lastValidToothTime = toothLastToothTime;

    emit_basic_distributor_gap(400U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(32U, 500U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(lastValidToothTime, toothLastToothTime);
    TEST_ASSERT_EQUAL_UINT32(1U, currentStatus.startRevolutions);

    emit_basic_distributor_gap(2000U);

    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(64U, 600U, triggerFilterTime);
}

void testBasicDistributor()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_basic_distributor_setup_uses_stroke_dependent_tooth_angle);
        RUN_TEST_P(test_basic_distributor_first_pulse_establishes_sync_then_counts_tooth_two);
        RUN_TEST_P(test_basic_distributor_wraps_on_fifth_pulse_for_four_cylinder_four_stroke);
        RUN_TEST_P(test_basic_distributor_short_gap_is_filtered_after_sync);
    }
}
