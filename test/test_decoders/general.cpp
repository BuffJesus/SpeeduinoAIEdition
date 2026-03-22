#include <unity.h>
#include "../test_utils.h"
#include "decoders.h"
#include "globals.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothSystemLastToothTime;

static void reset_decoder_timing_state(void)
{
    resetDecoder();
    testClearTriggerStateOverrides();
    currentStatus.hasSync = false;
    currentStatus.status3 = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage4.StgCycles = 0U;
    configPage4.triggerFilter = TRIGGER_FILTER_OFF;
    revolutionTime = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothSystemLastToothTime = 0U;
    triggerFilterTime = 0U;
    triggerSecFilterTime = 0U;
}

static void test_engineIsRunning(void)
{
    extern volatile unsigned long toothLastToothTime;
    extern unsigned long MAX_STALL_TIME;
  
    MAX_STALL_TIME = 1000;
    toothLastToothTime = 0;
    TEST_ASSERT_TRUE(engineIsRunning(toothLastToothTime+MAX_STALL_TIME-1UL));
    TEST_ASSERT_FALSE(engineIsRunning(toothLastToothTime+MAX_STALL_TIME));
    TEST_ASSERT_FALSE(engineIsRunning(toothLastToothTime+MAX_STALL_TIME+1UL));

    // Simulate an interrupt for a pulse being triggered between a call 
    // to micros() (1000) and the call to engineIsRunning()
    toothLastToothTime = 2500;
    TEST_ASSERT_TRUE(engineIsRunning(1000UL));

    TEST_ASSERT_TRUE(engineIsRunning(2499UL));
    TEST_ASSERT_TRUE(engineIsRunning(2500UL));
    TEST_ASSERT_TRUE(engineIsRunning(2501UL));

    TEST_ASSERT_FALSE(engineIsRunning(toothLastToothTime+MAX_STALL_TIME));
}

static void test_SetRevolutionTime_updates_only_on_change(void)
{
    revolutionTime = 1234U;

    TEST_ASSERT_FALSE(SetRevolutionTime(1234U));
    TEST_ASSERT_EQUAL_UINT32(1234U, revolutionTime);

    TEST_ASSERT_TRUE(SetRevolutionTime(4321U));
    TEST_ASSERT_EQUAL_UINT32(4321U, revolutionTime);
}

static void test_UpdateRevolutionTimeFromTeeth_requires_sync_and_noncranking_state(void)
{
    reset_decoder_timing_state();
    toothOneMinusOneTime = 1000U;
    toothOneTime = 7000U;

    TEST_ASSERT_FALSE(testUpdateRevolutionTimeFromTeeth(false));
    TEST_ASSERT_EQUAL_UINT32(0U, revolutionTime);

    currentStatus.hasSync = true;
    TEST_ASSERT_FALSE(testUpdateRevolutionTimeFromTeeth(false));
    TEST_ASSERT_EQUAL_UINT32(0U, revolutionTime);

    currentStatus.startRevolutions = 1U;
    TEST_ASSERT_TRUE(testUpdateRevolutionTimeFromTeeth(false));
    TEST_ASSERT_EQUAL_UINT32(6000U, revolutionTime);
}

static void test_UpdateRevolutionTimeFromTeeth_halves_cam_speed_period(void)
{
    reset_decoder_timing_state();
    currentStatus.hasSync = true;
    currentStatus.startRevolutions = 1U;
    toothOneMinusOneTime = 1000U;
    toothOneTime = 9000U;

    TEST_ASSERT_TRUE(testUpdateRevolutionTimeFromTeeth(true));
    TEST_ASSERT_EQUAL_UINT32(4000U, revolutionTime);
}

static void test_RpmFromRevolutionTimeUs_clamps_above_max_rpm(void)
{
    currentStatus.RPM = 3210U;

    TEST_ASSERT_EQUAL_UINT16(3000U, testRpmFromRevolutionTimeUs(20000U));
    TEST_ASSERT_EQUAL_UINT16(3210U, testRpmFromRevolutionTimeUs(10U));
}

static void test_crankingGetRPM_requires_staging_cycles_and_sync(void)
{
    reset_decoder_timing_state();
    currentStatus.RPM = 222U;
    configPage4.StgCycles = 2U;
    toothLastMinusOneToothTime = 1000U;
    toothLastToothTime = 2000U;

    TEST_ASSERT_EQUAL_INT(222, testCrankingGetRPM(36U, false));

    currentStatus.hasSync = true;
    currentStatus.startRevolutions = 1U;
    TEST_ASSERT_EQUAL_INT(222, testCrankingGetRPM(36U, false));

    currentStatus.startRevolutions = 2U;
    TEST_ASSERT_EQUAL_INT(1667, testCrankingGetRPM(36U, false));
    TEST_ASSERT_EQUAL_UINT32(36000U, revolutionTime);
}

static void test_crankingGetRPM_accepts_half_sync_and_cam_speed(void)
{
    reset_decoder_timing_state();
    currentStatus.RPM = 150U;
    configPage4.StgCycles = 0U;
    BIT_SET(currentStatus.status3, BIT_STATUS3_HALFSYNC);
    currentStatus.startRevolutions = 1U;
    toothLastMinusOneToothTime = 1000U;
    toothLastToothTime = 3000U;

    TEST_ASSERT_EQUAL_INT(1000, testCrankingGetRPM(60U, true));
    TEST_ASSERT_EQUAL_UINT32(60000U, revolutionTime);
}

static void test_setFilter_matches_trigger_filter_modes(void)
{
    configPage4.triggerFilter = TRIGGER_FILTER_OFF;
    testSetFilter(2000U);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);

    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    testSetFilter(2000U);
    TEST_ASSERT_EQUAL_UINT32(500U, triggerFilterTime);

    configPage4.triggerFilter = TRIGGER_FILTER_MEDIUM;
    testSetFilter(2000U);
    TEST_ASSERT_EQUAL_UINT32(1000U, triggerFilterTime);

    configPage4.triggerFilter = TRIGGER_FILTER_AGGRESSIVE;
    testSetFilter(2000U);
    TEST_ASSERT_EQUAL_UINT32(1500U, triggerFilterTime);
}

void testDecoder_General()
{
  SET_UNITY_FILENAME() {
    RUN_TEST_P(test_engineIsRunning);
    RUN_TEST_P(test_SetRevolutionTime_updates_only_on_change);
    RUN_TEST_P(test_UpdateRevolutionTimeFromTeeth_requires_sync_and_noncranking_state);
    RUN_TEST_P(test_UpdateRevolutionTimeFromTeeth_halves_cam_speed_period);
    RUN_TEST_P(test_RpmFromRevolutionTimeUs_clamps_above_max_rpm);
    RUN_TEST_P(test_crankingGetRPM_requires_staging_cycles_and_sync);
    RUN_TEST_P(test_crankingGetRPM_accepts_half_sync_and_cam_speed);
    RUN_TEST_P(test_setFilter_matches_trigger_filter_modes);
  }
}
