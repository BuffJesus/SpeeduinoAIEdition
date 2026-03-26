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

// Full-revolution test: anchors on double gap, drives through all 33 actual teeth
// and both gap events, asserting counts at each sync point.
// Physical layout: [count=1] -17 teeth-> [single gap] -16 teeth-> [double gap] ->[count=1]
void t3621_full_revolution(void)
{
    // Anchor on double gap to establish count=1
    setup_3621_state_machine();
    emit_3621_gap(2000U);
    emit_3621_gap(2000U);
    emit_3621_gap(7000U); // double gap (>3x): count=1
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    // Drive the 17-tooth run (count=1 is tooth 1; emit 16 more for tooth 2..17)
    for (uint8_t tooth = 0; tooth < 16U; ++tooth)
    {
        emit_3621_gap(2000U);
    }
    TEST_ASSERT_EQUAL_UINT16(17U, toothCurrentCount); // last tooth before single gap

    // Single gap (1.5x < ratio < 3x): first tooth after single gap is position 19
    emit_3621_gap(4000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    // Drive the 16-tooth run (count=19 is tooth 1; emit 15 more for tooth 2..16)
    for (uint8_t tooth = 0; tooth < 15U; ++tooth)
    {
        emit_3621_gap(2000U);
    }
    TEST_ASSERT_EQUAL_UINT16(34U, toothCurrentCount); // last tooth before double gap

    // Double gap (>3x): revolution complete, back to count=1
    emit_3621_gap(7000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(BIT_CHECK(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT));

    reset_3621_runtime();
}

// Self-correction test: spurious extra tooth before the single-gap event.
// The single-gap event re-anchors toothCurrentCount = 19 regardless of count drift.
// Physical layout used: count=1 (post-double-gap), 8 regular teeth, 1 spurious, 8 more
// regular teeth, then single gap. Count before gap is 18 (off by 1 vs expected 17),
// but the gap event still forces count = 19.
void t3621_single_gap_reanchors_count_after_spurious_tooth(void)
{
    // Establish sync via double gap (same path as t3621)
    setup_3621_state_machine();
    emit_3621_gap(2000U);
    emit_3621_gap(2000U);
    emit_3621_gap(7000U); // double gap -> count=1, hasSync=true
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    // Drive 8 regular teeth (count -> 9)
    for (uint8_t i = 0U; i < 8U; ++i) { emit_3621_gap(2000U); }
    TEST_ASSERT_EQUAL_UINT16(9U, toothCurrentCount);

    // Spurious extra tooth at normal spacing; count drifts to 10 (1 ahead of physical)
    emit_3621_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(10U, toothCurrentCount);

    // Drive 8 more regular teeth (count -> 18, 1 ahead of the expected 17)
    for (uint8_t i = 0U; i < 8U; ++i) { emit_3621_gap(2000U); }
    TEST_ASSERT_EQUAL_UINT16(18U, toothCurrentCount);

    // Single-gap event: count++ -> 19, gap detected (4000 > 1.5x2000=3000, < 3x2000=6000)
    // -> count forced to 19. Drift fully corrected.
    emit_3621_gap(4000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);

    reset_3621_runtime();
}

// Self-correction test: spurious extra tooth before the double-gap event.
// The double-gap event re-anchors toothCurrentCount = 1 regardless of count drift.
// Physical layout: count=1 (post-double-gap), 16 regular teeth, single gap -> count=19,
// 5 regular teeth, 1 spurious, 10 more regular teeth, then double gap. Count before gap
// is 35 (off by 1 vs expected 34), but the gap event still forces count = 1.
void t3621_double_gap_reanchors_count_after_spurious_tooth(void)
{
    // Establish sync via double gap then drive through single gap to count=19
    setup_3621_state_machine();
    emit_3621_gap(2000U);
    emit_3621_gap(2000U);
    emit_3621_gap(7000U); // double gap -> count=1, hasSync=true
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    for (uint8_t i = 0U; i < 16U; ++i) { emit_3621_gap(2000U); } // count -> 17
    emit_3621_gap(4000U); // single gap -> count=19
    TEST_ASSERT_EQUAL_UINT16(19U, toothCurrentCount);

    // Drive 5 regular teeth (count -> 24)
    for (uint8_t i = 0U; i < 5U; ++i) { emit_3621_gap(2000U); }
    TEST_ASSERT_EQUAL_UINT16(24U, toothCurrentCount);

    // Spurious extra tooth; count drifts to 25 (1 ahead of physical position 24)
    emit_3621_gap(2000U);
    TEST_ASSERT_EQUAL_UINT16(25U, toothCurrentCount);

    // Drive 10 more regular teeth (count -> 35, 1 ahead of the expected 34)
    for (uint8_t i = 0U; i < 10U; ++i) { emit_3621_gap(2000U); }
    TEST_ASSERT_EQUAL_UINT16(35U, toothCurrentCount);

    // Double-gap event: count++ -> 36, gap detected (7000 > 3x2000=6000)
    // -> count forced to 1. Drift fully corrected.
    emit_3621_gap(7000U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    reset_3621_runtime();
}

void testThirtySixMinus21()
{
    RUN_TEST_P(t3621);
    RUN_TEST_P(t3621_full_revolution);
    RUN_TEST_P(t3621_single_gap_reanchors_count_after_spurious_tooth);
    RUN_TEST_P(t3621_double_gap_reanchors_count_after_spurious_tooth);
}
