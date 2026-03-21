#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "SuzukiK6A.h"
#include "../../test_utils.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile unsigned long triggerFilterTime;

static void reset_k6a_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage2.nCylinders = 3U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    configPage4.TrigPattern = DECODER_SUZUKI_K6A;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    resetDecoder();
}

static void setup_k6a_state_machine(void)
{
    reset_k6a_runtime();
    triggerSetup_SuzukiK6A();
}

static void emit_k6a_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_SuzukiK6A();
}

static void establish_k6a_sync(unsigned long longGapUs = 3000U,
                               unsigned long mediumGapUs = 2000U,
                               unsigned long syncGapUs = 1000U)
{
    emit_k6a_primary_gap(longGapUs);
    emit_k6a_primary_gap(mediumGapUs);
    emit_k6a_primary_gap(syncGapUs);
}

static void test_k6a_state_descending_gaps_establish_sync_on_sync_tooth(void)
{
    setup_k6a_state_machine();

    establish_k6a_sync();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(64U, 1000U, triggerFilterTime);
}

static void test_k6a_state_synced_cycle_wraps_and_counts_two_revolutions(void)
{
    setup_k6a_state_machine();

    establish_k6a_sync();
    emit_k6a_primary_gap(4000U);
    emit_k6a_primary_gap(2000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

static void test_k6a_state_short_gap_after_sync_is_filtered(void)
{
    setup_k6a_state_machine();

    establish_k6a_sync();
    emit_k6a_primary_gap(4000U);
    emit_k6a_primary_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    emit_k6a_primary_gap(500U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.startRevolutions);
}

void testSuzukiK6A_runtimeState(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST(test_k6a_state_descending_gaps_establish_sync_on_sync_tooth);
        RUN_TEST(test_k6a_state_synced_cycle_wraps_and_counts_two_revolutions);
        RUN_TEST(test_k6a_state_short_gap_after_sync_is_filtered);
    }
}
