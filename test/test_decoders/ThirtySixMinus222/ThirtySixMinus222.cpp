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

// Full-revolution test: from double-gap sync (count=19), drive through the
// single-gap group (count=35), wrap to count=1, then re-establish double-gap
// sync (count=19) to complete one full crank revolution on the H4 path.
//
// Physical layout (4-cyl path, nominal tooth positions):
//   Positions 1-2  : regular (count=1..2 from wrap, then double-gap group)
//   Positions 3-18 : double-gap group A → sync count=19
//   Positions 20-31: 12 regular teeth
//   Positions 32-33: single-gap group B (long+regular) → sync count=35
//   Positions 34-36: 3 regular teeth (count 36, 37→wrap to 1)
static void test_36222_h4_full_revolution(void)
{
    // Establish double-gap sync → count=19
    setup_36222_state_machine();
    emit_36222_gap(2000U);
    emit_36222_gap(2000U);
    emit_36222_gap(6000U); // first long gap of group A
    emit_36222_gap(6000U); // second long gap of group A → count=19, hasSync=true
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);

    // Drive 12 regular teeth (count → 31)
    for (uint8_t i = 0U; i < 12U; ++i) { emit_36222_gap(2000U); }
    TEST_ASSERT_EQUAL_UINT16(31U, toothCurrentCount);

    // Single-gap group B: long gap → count++ to 32, +=2 to 34, toothSystemCount=1
    emit_36222_gap(6000U);
    TEST_ASSERT_EQUAL_UINT16(34U, toothCurrentCount);

    // Regular tooth after long gap → count=35 (single-gap re-sync), toothSystemCount=0
    emit_36222_gap(2000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(35U, toothCurrentCount);

    // count=36 (regular, no wrap: 36 not > 36)
    emit_36222_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(36U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);

    // count=37 > 36 → wrap to count=1, startRevolutions++
    emit_36222_gap(2000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);

    // From count=1: 2 regular + double-gap → re-sync at count=19
    emit_36222_gap(2000U); // count=2
    emit_36222_gap(2000U); // count=3
    emit_36222_gap(6000U); // first long gap of group A (next revolution)
    emit_36222_gap(6000U); // second long gap → count=19, hasSync=true
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);

    reset_36222_runtime();
}

// Self-correction: a spurious extra tooth before the double-gap group shifts
// the count by 1, but the double-gap event re-anchors count=19 unconditionally.
static void test_36222_h4_double_gap_reanchors_count_after_spurious_tooth(void)
{
    setup_36222_state_machine();

    // Warm up two teeth (count → 2)
    emit_36222_gap(2000U);
    emit_36222_gap(2000U);

    // Spurious extra tooth at normal spacing (count → 3, off by 1)
    emit_36222_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);

    // Double-gap group fires: regardless of prior count, count is forced to 19
    emit_36222_gap(6000U); // first long gap
    emit_36222_gap(6000U); // second long gap → count=19
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);

    reset_36222_runtime();
}

// Self-correction: a spurious extra tooth before the single-gap group shifts
// the count by 1, but the single-gap event re-anchors count=35 unconditionally.
static void test_36222_h4_single_gap_reanchors_count_after_spurious_tooth(void)
{
    setup_36222_state_machine();

    // Warm up two teeth (count → 2)
    emit_36222_gap(2000U);
    emit_36222_gap(2000U);

    // Spurious extra tooth at normal spacing (count → 3, off by 1)
    emit_36222_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(3U, toothCurrentCount);

    // Single-gap group: long gap then regular → count=35 regardless of prior count
    emit_36222_gap(6000U); // first long gap
    emit_36222_gap(2000U); // regular tooth after lone gap → count=35
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(35U, toothCurrentCount);

    reset_36222_runtime();
}

void testThirtySixMinus222()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_36222_h4_double_gap_path_syncs_tooth19);
        RUN_TEST_P(test_36222_h4_single_gap_path_syncs_tooth35);
        RUN_TEST_P(test_36222_h4_wrap_counts_revolution);
        RUN_TEST_P(test_36222_h4_full_revolution);
        RUN_TEST_P(test_36222_h4_double_gap_reanchors_count_after_spurious_tooth);
        RUN_TEST_P(test_36222_h4_single_gap_reanchors_count_after_spurious_tooth);
    }
}
