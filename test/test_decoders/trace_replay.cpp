#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "trace_fixture.h"
#include "traces/dual_wheel_sync_trace.h"
#include "traces/dual_wheel_short_secondary_trace.h"
#include "traces/dual_wheel_secondary_noise_trace.h"
#include "traces/dual_wheel_wrap_trace.h"
#include "traces/dual_wheel_resync_trace.h"
#include "traces/fordst170_cam_regroup_trace.h"
#include "traces/k6a_post_sync_short_gap_trace.h"
#include "traces/k6a_sync_trace.h"
#include "traces/k6a_sync_noise_trace.h"
#include "traces/k6a_wrap_trace.h"
#include "traces/fordst170_cam_sync_trace.h"
#include "traces/fordst170_cam_noise_trace.h"
#include "traces/fordst170_primary_only_trace.h"
#include "traces/missing_tooth_36_1_early_gap_trace.h"
#include "traces/missing_tooth_36_1_half_sync_trace.h"
#include "traces/missing_tooth_36_1_noise_trace.h"
#include "traces/missing_tooth_36_1_sequential_cam_trace.h"
#include "traces/ngc4_primary_missing_tooth_trace.h"
#include "traces/nissan360_6cyl_window_4_trace.h"
#include "traces/nissan360_8cyl_window_7_trace.h"
#include "traces/nissan360_invalid_window_6_trace.h"
#include "traces/nissan360_window_16_trace.h"
#include "traces/nissan360_window_12_trace.h"
#include "traces/nissan360_window_8_trace.h"
#include "traces/nissan360_window_4_trace.h"
#include "traces/nissan360_wrap_trace.h"
#include "traces/renix_short_gap_after_group_trace.h"
#include "traces/renix_valid_group_trace.h"
#include "traces/miata9905_single_cam_trace.h"
#include "traces/miata9905_double_cam_sync_trace.h"
#include "traces/miata9905_cam_noise_trace.h"
#include "traces/jeep2000_cam_sync_trace.h"
#include "traces/jeep2000_full_revolution_trace.h"
#include "traces/jeep2000_no_cam_trace.h"
#include "traces/audi135_cam_sync_trace.h"
#include "traces/audi135_full_revolution_trace.h"
#include "traces/audi135_no_cam_trace.h"
#include "traces/audi135_resync_trace.h"
#include "traces/gm24x_cam_sync_trace.h"
#include "traces/gm24x_full_revolution_trace.h"
#include "traces/gm24x_no_cam_trace.h"
#include "traces/gm24x_resync_trace.h"
#include "traces/4g63_cam_sync_trace.h"
#include "traces/4g63_no_cam_trace.h"
#include "traces/4g63_sync_loss_trace.h"
#include "traces/gm7x_sync_trace.h"
#include "traces/gm7x_no_sync_trace.h"
#include "traces/gm7x_wrap_trace.h"
#include "traces/hondad17_sync_trace.h"
#include "traces/hondad17_no_sync_trace.h"
#include "traces/hondad17_wrap_trace.h"
#include "../test_utils.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long toothLastToothRisingTime;
extern volatile unsigned long toothLastSecToothRisingTime;
extern volatile unsigned long toothSystemLastToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_trace_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothLastToothRisingTime = 0U;
    toothLastSecToothRisingTime = 0U;
    toothSystemLastToothTime = 0U;
    toothCurrentCount = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_trace_k6a(void)
{
    reset_trace_runtime();
    configPage4.TrigPattern = DECODER_SUZUKI_K6A;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 3U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_SuzukiK6A();
}

static void setup_trace_dualwheel(void)
{
    configPage4.triggerTeeth = 12U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.StgCycles = 0U;
    configPage4.useResync = 0U;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.perToothIgn = false;
    triggerSetup_DualWheel();
    reset_trace_runtime();
}

static void setup_trace_nissan360(uint8_t cylinderCount = 4U)
{
    reset_trace_runtime();
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 0U;
    configPage4.TrigEdgeSec = 0U;
    configPage2.nCylinders = cylinderCount;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Nissan360();
}

static void setup_trace_ngc4(void)
{
    reset_trace_runtime();
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage4.triggerAngle = 0;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.nCylinders = 4U;
    configPage2.perToothIgn = false;
    toothCurrentCount = 0U;
    toothLastMinusOneToothTime = 0U;
    toothLastMinusOneSecToothTime = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    revolutionOne = false;
    resetDecoder();
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinTrigger2, OUTPUT);
    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinTrigger2, LOW);
    triggerSetup_NGC();
    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinTrigger2, LOW);
}

static void setup_trace_renix44(void)
{
    reset_trace_runtime();
    configPage4.TrigPattern = DECODER_RENIX;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Renix();
}

static void setup_trace_miata9905(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CAM_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 1U;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Miata9905();
}

static void setup_trace_jeep2000(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 6U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Jeep2000();
}

static void setup_trace_audi135(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 0U;
    configPage2.nCylinders = 5U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Audi135();
}

static void setup_trace_audi135_with_resync(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 1U;
    configPage2.nCylinders = 5U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_Audi135();
}

static void setup_trace_missing_tooth_36_1(void)
{
    reset_trace_runtime();
    configPage4.triggerTeeth = 36U;
    configPage4.triggerMissingTeeth = 1U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_missingTooth();
}

static void setup_trace_gm24x(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 8U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_24X();
}

static void setup_trace_4g63(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 0U;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinTrigger2, OUTPUT);
    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinTrigger2, LOW);
    triggerSetup_4G63();
    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinTrigger2, LOW);
}

static void setup_trace_gm7x(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 6U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_GM7X();
}

static void setup_trace_hondad17(void)
{
    reset_trace_runtime();
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_HondaD17();
    toothLastToothTime = micros();
}

static void setup_trace_missing_tooth_36_1_sequential(void)
{
    reset_trace_runtime();
    configPage4.triggerTeeth = 36U;
    configPage4.triggerMissingTeeth = 1U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    triggerSetup_missingTooth();
}

static void setup_trace_fordst170(void)
{
    reset_trace_runtime();
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.nCylinders = 4U;
    configPage2.perToothIgn = false;
    configPage6.vvtEnabled = 0U;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    triggerSetup_FordST170();
}

static void triggerSec_Nissan360_rising(void)
{
    configPage4.TrigEdgeSec = 0U;
    triggerSec_Nissan360();
}

static void triggerSec_Nissan360_falling(void)
{
    configPage4.TrigEdgeSec = 1U;
    triggerSec_Nissan360();
}

static TraceReplayCallbacks makeNissan360Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_Nissan360, nullptr, triggerSec_Nissan360_rising, triggerSec_Nissan360_falling};
    return callbacks;
}

static TraceReplayCallbacks makeFordSt170Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_missingTooth, nullptr, triggerSec_FordST170, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makeDualWheelCallbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_DualWheel, nullptr, triggerSec_DualWheel, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makeMissingToothCallbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_missingTooth, nullptr, triggerSec_missingTooth, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makePrimaryOnlyCallbacks(void (*primary)())
{
    const TraceReplayCallbacks callbacks = {primary, nullptr, nullptr, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makeMiata9905Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_Miata9905, nullptr, triggerSec_Miata9905, triggerSec_Miata9905};
    return callbacks;
}

static TraceReplayCallbacks makeJeep2000Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_Jeep2000, nullptr, triggerSec_Jeep2000, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makeAudi135Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_Audi135, nullptr, triggerSec_Audi135, nullptr};
    return callbacks;
}

static TraceReplayCallbacks makeGm24XCallbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_24X, nullptr, triggerSec_24X, nullptr};
    return callbacks;
}

static void triggerPri_4g63_rising_trace(void)
{
    digitalWrite(pinTrigger, HIGH);
    triggerPri_4G63();
}

static void triggerPri_4g63_falling_trace(void)
{
    digitalWrite(pinTrigger, LOW);
    triggerPri_4G63();
}

static void triggerSec_4g63_rising_trace(void)
{
    digitalWrite(pinTrigger2, HIGH);
    triggerSec_4G63();
}

static void triggerSec_4g63_falling_trace(void)
{
    digitalWrite(pinTrigger2, LOW);
    triggerSec_4G63();
}

static TraceReplayCallbacks make4G63Callbacks(void)
{
    const TraceReplayCallbacks callbacks = {triggerPri_4g63_rising_trace, triggerPri_4g63_falling_trace, triggerSec_4g63_rising_trace, triggerSec_4g63_falling_trace};
    return callbacks;
}

static void triggerPri_NGC_normal_trace(void)
{
    toothLastToothRisingTime = micros() - 1200U;
    digitalWrite(pinTrigger, LOW);
    triggerPri_NGC();
}

static void triggerPri_NGC_missing_high_trace(void)
{
    toothLastToothRisingTime = micros() - 4200U;
    digitalWrite(pinTrigger, LOW);
    triggerPri_NGC();
}

static void test_trace_replay_k6a_primary_only_trace_survives_noise(void)
{
    setup_trace_k6a();

    replayTriggerTrace(makeTriggerTrace(kK6aSyncNoiseEvents), makePrimaryOnlyCallbacks(triggerPri_SuzukiK6A));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
}

static void test_trace_replay_k6a_descending_gaps_establish_sync_on_sync_tooth(void)
{
    setup_trace_k6a();

    replayTriggerTrace(makeTriggerTrace(kK6aSyncEvents), makePrimaryOnlyCallbacks(triggerPri_SuzukiK6A));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_UINT32_WITHIN(64U, 1000U, triggerFilterTime);
}

static void test_trace_replay_k6a_wraps_and_counts_two_revolutions(void)
{
    setup_trace_k6a();

    replayTriggerTrace(makeTriggerTrace(kK6aWrapEvents), makePrimaryOnlyCallbacks(triggerPri_SuzukiK6A));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

static void test_trace_replay_k6a_short_gap_after_sync_is_filtered(void)
{
    setup_trace_k6a();

    replayTriggerTrace(makeTriggerTrace(kK6aPostSyncShortGapEvents), makePrimaryOnlyCallbacks(triggerPri_SuzukiK6A));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(2U, currentStatus.startRevolutions);
}

static void test_trace_replay_dualwheel_secondary_noise_filters_and_still_wraps(void)
{
    setup_trace_dualwheel();
    revolutionTime = 8000U;

    replayTriggerTrace(makeTriggerTrace(kDualWheelSecondaryNoiseEvents), makeDualWheelCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT32(revolutionTime >> 1, triggerSecFilterTime);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_dualwheel_secondary_pulse_establishes_sync_and_primes_primary(void)
{
    setup_trace_dualwheel();

    replayTriggerTrace(makeTriggerTrace(kDualWheelSyncEvents), makeDualWheelCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);
}

static void test_trace_replay_dualwheel_short_secondary_gap_is_filtered(void)
{
    setup_trace_dualwheel();
    revolutionTime = 8000U;
    triggerSecFilterTime = 5000U;

    replayTriggerTrace(makeTriggerTrace(kDualWheelShortSecondaryEvents), makeDualWheelCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(revolutionTime >> 1, triggerSecFilterTime);
}

static void test_trace_replay_dualwheel_first_primary_after_sync_wraps_and_counts_revolution(void)
{
    setup_trace_dualwheel();

    replayTriggerTrace(makeTriggerTrace(kDualWheelWrapEvents), makeDualWheelCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(triggerFilterTime >= 750U);
    TEST_ASSERT_TRUE(triggerFilterTime < 1000U);
}

static void test_trace_replay_dualwheel_resync_counts_sync_loss_and_resets_tooth(void)
{
    setup_trace_dualwheel();
    configPage4.useResync = 1U;
    currentStatus.hasSync = true;
    currentStatus.startRevolutions = 3U;
    toothCurrentCount = 3U;
    triggerSecFilterTime = 1000U;

    replayTriggerTrace(makeTriggerTrace(kDualWheelResyncEvents), makeDualWheelCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
}

static void test_trace_replay_nissan360_mixed_channel_trace_syncs(void)
{
    setup_trace_nissan360();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360Window16Events), makeNissan360Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);
}

static void test_trace_replay_nissan360_other_4cyl_windows_sync(void)
{
    setup_trace_nissan360();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360Window12Events), makeNissan360Callbacks());
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(102U, toothCurrentCount);

    setup_trace_nissan360();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360Window8Events), makeNissan360Callbacks());
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(188U, toothCurrentCount);

    setup_trace_nissan360();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360Window4Events), makeNissan360Callbacks());
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(274U, toothCurrentCount);
}

static void test_trace_replay_nissan360_invalid_window_does_not_sync(void)
{
    setup_trace_nissan360();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360InvalidWindow6Events), makeNissan360Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);
}

static void test_trace_replay_nissan360_other_supported_cylinder_windows_sync(void)
{
    setup_trace_nissan360(6U);
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360SixCylinderWindow4Events), makeNissan360Callbacks());
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(124U, toothCurrentCount);

    setup_trace_nissan360(8U);
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360EightCylinderWindow7Events), makeNissan360Callbacks());
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(56U, toothCurrentCount);
}

static void test_trace_replay_nissan360_resync_window_realigns_tooth_counter(void)
{
    setup_trace_nissan360();
    currentStatus.hasSync = true;
    configPage4.useResync = true;
    toothCurrentCount = 50U;

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360Window16Events), makeNissan360Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);
}

static void test_trace_replay_nissan360_wraps_and_counts_revolution(void)
{
    setup_trace_nissan360();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kNissan360WrapEvents), makeNissan360Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

static void test_trace_replay_renix_valid_group_advances_virtual_tooth_state(void)
{
    setup_trace_renix44();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kRenixValidGroupEvents), makePrimaryOnlyCallbacks(triggerPri_Renix));

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
}

static void test_trace_replay_renix_short_gap_after_group_is_filtered(void)
{
    setup_trace_renix44();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kRenixShortGapAfterGroupEvents), makePrimaryOnlyCallbacks(triggerPri_Renix));

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
}

static void test_trace_replay_miata9905_single_cam_pulse_establishes_sync(void)
{
    setup_trace_miata9905();

    replayTriggerTrace(makeTriggerTrace(kMiata9905SingleCamEvents), makeMiata9905Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, testGetSecondaryToothCount());  // Reset at end of last primary
    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);  // Tooth 3 became 6, then tooth 4 incremented to 7
}

static void test_trace_replay_miata9905_sync_then_wrap_counts_revolution(void)
{
    setup_trace_miata9905();

    replayTriggerTrace(makeTriggerTrace(kMiata9905CamSyncWrapEvents), makeMiata9905Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_miata9905_extra_cam_pulse_defers_sync_until_clean_pulse(void)
{
    setup_trace_miata9905();

    replayTriggerTrace(makeTriggerTrace(kMiata9905CamNoiseEvents), makeMiata9905Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT8(0U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_trace_replay_jeep2000_cam_pulse_resets_tooth_count(void)
{
    setup_trace_jeep2000();

    replayTriggerTrace(makeTriggerTrace(kJeep2000CamSyncEvents), makeJeep2000Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);  // Cam reset to 0, first primary sets to 1, but filter prevents next teeth
}

static void test_trace_replay_jeep2000_full_revolution_wraps(void)
{
    setup_trace_jeep2000();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kJeep2000FullRevolutionEvents), makeJeep2000Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);  // After cam reset and wrap
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_jeep2000_no_cam_stays_without_sync(void)
{
    setup_trace_jeep2000();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kJeep2000NoCamEvents), makeJeep2000Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
}

static void test_trace_replay_audi135_cam_establishes_sync_and_decimates_teeth(void)
{
    setup_trace_audi135();

    replayTriggerTrace(makeTriggerTrace(kAudi135CamSyncEvents), makeAudi135Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);  // Cam set to 0, next 3 physical teeth = 1 effective tooth
}

static void test_trace_replay_audi135_full_revolution_trace_preserves_sync_without_false_wrap(void)
{
    setup_trace_audi135();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kAudi135FullRevolutionEvents), makeAudi135Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);  // Current trace only establishes the first effective tooth
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_audi135_no_cam_stays_without_sync(void)
{
    setup_trace_audi135();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kAudi135NoCamEvents), makeAudi135Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(255U, toothCurrentCount);  // Stays at UINT8_MAX without sync
}

static void test_trace_replay_audi135_resync_holds_tooth_zero_until_next_valid_primary(void)
{
    setup_trace_audi135_with_resync();

    replayTriggerTrace(makeTriggerTrace(kAudi135ResyncEvents), makeAudi135Callbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);  // Resync pulse resets to zero and this trace does not yet reach the next accepted effective tooth
}

static void test_trace_replay_gm24x_cam_pulse_establishes_sync(void)
{
    setup_trace_gm24x();

    replayTriggerTrace(makeTriggerTrace(kGm24xCamSyncEvents), makeGm24XCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_gm24x_full_revolution_wraps_on_next_cam(void)
{
    setup_trace_gm24x();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kGm24xFullRevolutionEvents), makeGm24XCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
}

static void test_trace_replay_gm24x_no_cam_stays_without_sync(void)
{
    setup_trace_gm24x();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kGm24xNoCamEvents), makeGm24XCallbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(25U, toothCurrentCount);
}

static void test_trace_replay_gm24x_resync_resets_tooth_count(void)
{
    setup_trace_gm24x();

    replayTriggerTrace(makeTriggerTrace(kGm24xResyncEvents), makeGm24XCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
}

static void test_trace_replay_4g63_cam_phase_marks_candidate_tooth_without_false_sync(void)
{
    setup_trace_4g63();
    digitalWrite(pinTrigger2, HIGH);

    replayTriggerTrace(makeTriggerTrace(k4G63CamSyncEvents), make4G63Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_4g63_primary_only_stays_without_sync(void)
{
    setup_trace_4g63();

    replayTriggerTrace(makeTriggerTrace(k4G63NoCamEvents), make4G63Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_4g63_extra_cam_edge_does_not_false_sync(void)
{
    setup_trace_4g63();
    digitalWrite(pinTrigger2, HIGH);

    replayTriggerTrace(makeTriggerTrace(k4G63SyncLossEvents), make4G63Callbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);
}

static void test_trace_replay_gm7x_short_gap_establishes_sync(void)
{
    setup_trace_gm7x();

    replayTriggerTrace(makeTriggerTrace(kGm7xSyncEvents), makePrimaryOnlyCallbacks(triggerPri_GM7X));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_gm7x_uniform_gaps_do_not_sync(void)
{
    setup_trace_gm7x();

    replayTriggerTrace(makeTriggerTrace(kGm7xNoSyncEvents), makePrimaryOnlyCallbacks(triggerPri_GM7X));

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(4U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_trace_replay_gm7x_synced_cycle_wraps_after_tooth_seven(void)
{
    setup_trace_gm7x();

    replayTriggerTrace(makeTriggerTrace(kGm7xWrapEvents), makePrimaryOnlyCallbacks(triggerPri_GM7X));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_hondad17_short_gap_establishes_sync(void)
{
    setup_trace_hondad17();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kHondaD17SyncEvents), makePrimaryOnlyCallbacks(triggerPri_HondaD17));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
}

static void test_trace_replay_hondad17_uniform_gaps_do_not_sync(void)
{
    setup_trace_hondad17();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kHondaD17NoSyncEvents), makePrimaryOnlyCallbacks(triggerPri_HondaD17));

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(13U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_trace_replay_hondad17_synced_cycle_wraps_on_next_reference_tooth(void)
{
    setup_trace_hondad17();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kHondaD17WrapEvents), makePrimaryOnlyCallbacks(triggerPri_HondaD17));

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(2U, currentStatus.startRevolutions);
}

static void test_trace_replay_missing_tooth_36_1_noise_still_syncs(void)
{
    setup_trace_missing_tooth_36_1();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kMissingTooth36Minus1NoiseEvents), makeMissingToothCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_missing_tooth_36_1_clean_gap_syncs(void)
{
    setup_trace_missing_tooth_36_1();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kMissingTooth36Minus1HalfSyncEvents), makeMissingToothCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_missing_tooth_36_1_sequential_cam_promotes_full_sync(void)
{
    setup_trace_missing_tooth_36_1_sequential();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kMissingTooth36Minus1SequentialCamEvents), makeMissingToothCallbacks());

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_TRUE(currentStatus.startRevolutions > 0U);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_missing_tooth_36_1_early_gap_drops_sync(void)
{
    setup_trace_missing_tooth_36_1();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kMissingTooth36Minus1EarlyGapEvents), makeMissingToothCallbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
}

static void test_trace_replay_missing_tooth_36_1_primary_only_sequential_stays_half_sync(void)
{
    setup_trace_missing_tooth_36_1_sequential();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kMissingTooth36Minus1HalfSyncEvents), makeMissingToothCallbacks());

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_fordst170_cam_noise_still_full_syncs(void)
{
    setup_trace_fordst170();

    const TraceReplayCallbacks callbacks = makeFordSt170Callbacks();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kFordSt170CamNoiseEvents), callbacks);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_fordst170_clean_cam_before_gap_full_syncs(void)
{
    setup_trace_fordst170();

    const TraceReplayCallbacks callbacks = makeFordSt170Callbacks();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kFordSt170CamSyncEvents), callbacks);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_fordst170_primary_only_sets_half_sync(void)
{
    setup_trace_fordst170();

    const TraceReplayCallbacks callbacks = makeFordSt170Callbacks();
    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kFordSt170PrimaryOnlyEvents), callbacks);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_fordst170_sixth_cam_tooth_restarts_grouping(void)
{
    setup_trace_fordst170();

    replayRepeatedTriggerTrace(makeRepeatedTriggerTrace(kFordSt170CamRegroupEvents), makeFordSt170Callbacks());

    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerSecFilterTime);
    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(0U, toothCurrentCount);
}

static void test_trace_replay_ngc4_primary_only_trace_sets_half_sync(void)
{
    setup_trace_ngc4();

    const TraceReplayCallbacks callbacks = {triggerPri_NGC_normal_trace, triggerPri_NGC_missing_high_trace, nullptr, nullptr};
    replayTriggerTrace(makeTriggerTrace(kNgc4PrimaryMissingToothEvents), callbacks);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_ngc4_phase_one_trace_establishes_full_sync(void)
{
    setup_trace_ngc4();
    testSetSecondaryToothCount(1U);

    const TraceReplayCallbacks callbacks = {triggerPri_NGC_normal_trace, triggerPri_NGC_missing_high_trace, nullptr, nullptr};
    replayTriggerTrace(makeTriggerTrace(kNgc4PrimaryMissingToothEvents), callbacks);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

static void test_trace_replay_ngc4_phase_five_trace_establishes_opposite_revolution(void)
{
    setup_trace_ngc4();
    testSetSecondaryToothCount(5U);

    const TraceReplayCallbacks callbacks = {triggerPri_NGC_normal_trace, triggerPri_NGC_missing_high_trace, nullptr, nullptr};
    replayTriggerTrace(makeTriggerTrace(kNgc4PrimaryMissingToothEvents), callbacks);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void testTriggerTraceReplay(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_trace_replay_k6a_descending_gaps_establish_sync_on_sync_tooth);
        RUN_TEST_P(test_trace_replay_k6a_primary_only_trace_survives_noise);
        RUN_TEST_P(test_trace_replay_k6a_short_gap_after_sync_is_filtered);
        RUN_TEST_P(test_trace_replay_k6a_wraps_and_counts_two_revolutions);
        RUN_TEST_P(test_trace_replay_dualwheel_secondary_pulse_establishes_sync_and_primes_primary);
        RUN_TEST_P(test_trace_replay_dualwheel_short_secondary_gap_is_filtered);
        RUN_TEST_P(test_trace_replay_dualwheel_first_primary_after_sync_wraps_and_counts_revolution);
        RUN_TEST_P(test_trace_replay_dualwheel_secondary_noise_filters_and_still_wraps);
        RUN_TEST_P(test_trace_replay_dualwheel_resync_counts_sync_loss_and_resets_tooth);
        RUN_TEST_P(test_trace_replay_fordst170_clean_cam_before_gap_full_syncs);
        RUN_TEST_P(test_trace_replay_fordst170_cam_noise_still_full_syncs);
        RUN_TEST_P(test_trace_replay_fordst170_primary_only_sets_half_sync);
        RUN_TEST_P(test_trace_replay_fordst170_sixth_cam_tooth_restarts_grouping);
        RUN_TEST_P(test_trace_replay_missing_tooth_36_1_clean_gap_syncs);
        RUN_TEST_P(test_trace_replay_missing_tooth_36_1_early_gap_drops_sync);
        RUN_TEST_P(test_trace_replay_missing_tooth_36_1_noise_still_syncs);
        RUN_TEST_P(test_trace_replay_missing_tooth_36_1_primary_only_sequential_stays_half_sync);
        RUN_TEST_P(test_trace_replay_missing_tooth_36_1_sequential_cam_promotes_full_sync);
        RUN_TEST_P(test_trace_replay_ngc4_primary_only_trace_sets_half_sync);
        RUN_TEST_P(test_trace_replay_ngc4_phase_one_trace_establishes_full_sync);
        RUN_TEST_P(test_trace_replay_ngc4_phase_five_trace_establishes_opposite_revolution);
        RUN_TEST_P(test_trace_replay_nissan360_invalid_window_does_not_sync);
        RUN_TEST_P(test_trace_replay_nissan360_other_4cyl_windows_sync);
        RUN_TEST_P(test_trace_replay_nissan360_other_supported_cylinder_windows_sync);
        RUN_TEST_P(test_trace_replay_nissan360_resync_window_realigns_tooth_counter);
        RUN_TEST_P(test_trace_replay_nissan360_mixed_channel_trace_syncs);
        RUN_TEST_P(test_trace_replay_nissan360_wraps_and_counts_revolution);
        RUN_TEST_P(test_trace_replay_renix_short_gap_after_group_is_filtered);
        RUN_TEST_P(test_trace_replay_renix_valid_group_advances_virtual_tooth_state);
        RUN_TEST_P(test_trace_replay_miata9905_single_cam_pulse_establishes_sync);
        RUN_TEST_P(test_trace_replay_miata9905_sync_then_wrap_counts_revolution);
        RUN_TEST_P(test_trace_replay_miata9905_extra_cam_pulse_defers_sync_until_clean_pulse);
        RUN_TEST_P(test_trace_replay_jeep2000_cam_pulse_resets_tooth_count);
        RUN_TEST_P(test_trace_replay_jeep2000_full_revolution_wraps);
        RUN_TEST_P(test_trace_replay_jeep2000_no_cam_stays_without_sync);
        RUN_TEST_P(test_trace_replay_audi135_cam_establishes_sync_and_decimates_teeth);
        RUN_TEST_P(test_trace_replay_audi135_full_revolution_trace_preserves_sync_without_false_wrap);
        RUN_TEST_P(test_trace_replay_audi135_no_cam_stays_without_sync);
        RUN_TEST_P(test_trace_replay_audi135_resync_holds_tooth_zero_until_next_valid_primary);
        RUN_TEST_P(test_trace_replay_gm24x_cam_pulse_establishes_sync);
        RUN_TEST_P(test_trace_replay_gm24x_full_revolution_wraps_on_next_cam);
        RUN_TEST_P(test_trace_replay_gm24x_no_cam_stays_without_sync);
        RUN_TEST_P(test_trace_replay_gm24x_resync_resets_tooth_count);
        RUN_TEST_P(test_trace_replay_4g63_cam_phase_marks_candidate_tooth_without_false_sync);
        RUN_TEST_P(test_trace_replay_4g63_primary_only_stays_without_sync);
        RUN_TEST_P(test_trace_replay_4g63_extra_cam_edge_does_not_false_sync);
        RUN_TEST_P(test_trace_replay_gm7x_short_gap_establishes_sync);
        RUN_TEST_P(test_trace_replay_gm7x_uniform_gaps_do_not_sync);
        RUN_TEST_P(test_trace_replay_gm7x_synced_cycle_wraps_after_tooth_seven);
        RUN_TEST_P(test_trace_replay_hondad17_short_gap_establishes_sync);
        RUN_TEST_P(test_trace_replay_hondad17_uniform_gaps_do_not_sync);
        RUN_TEST_P(test_trace_replay_hondad17_synced_cycle_wraps_on_next_reference_tooth);
    }
}
