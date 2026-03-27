#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "../test_utils.h"
#include "../decoder_test_utils.h"
#include "Miata9905.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile unsigned long triggerFilterTime;
extern volatile unsigned long triggerSecFilterTime;
extern volatile uint16_t triggerToothAngle;

static void reset_miata9905_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    currentStatus.initialisationComplete = false;

    configPage4.TrigPattern = DECODER_MIATA_9905;
    configPage4.TrigSpeed = CAM_SPEED;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.useResync = 1U;
    configPage4.ignCranklock = 0U;
    configPage2.nCylinders = 4U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    triggerFilterTime = 0U;
    triggerSecFilterTime = 0U;
    toothOneTime = 0U;
    toothOneMinusOneTime = 0U;
    resetDecoderTestTransientState();
}

static void setup_miata9905_state_machine(void)
{
    reset_miata9905_runtime();
    triggerSetup_Miata9905();
    toothLastToothTime = micros();
    testSetLastSecondaryToothTime(micros());
}

static void emit_miata9905_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Miata9905();
}

static void emit_miata9905_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_Miata9905();
}

static void test_miata9905_setup_seeds_unsynced_sentinel_and_filters(void)
{
    setup_miata9905_state_machine();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(99U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(90U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT32(1500U, triggerFilterTime);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerSecFilterTime);
}

static void test_miata9905_two_cam_edges_sync_next_primary_tooth_six(void)
{
    setup_miata9905_state_machine();

    emit_miata9905_primary_gap(19000U);
    emit_miata9905_primary_gap(30000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(101U, toothCurrentCount);

    emit_miata9905_secondary_gap(15000U);
    emit_miata9905_secondary_gap(5000U);

    TEST_ASSERT_EQUAL_UINT16(2U, testGetSecondaryToothCount());

    emit_miata9905_primary_gap(4000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
    TEST_ASSERT_EQUAL_UINT16(110U, triggerToothAngle);
    TEST_ASSERT_EQUAL_UINT16(0U, currentStatus.startRevolutions);
}

static void test_miata9905_extra_cam_edges_defer_sync_until_clean_pair(void)
{
    setup_miata9905_state_machine();

    emit_miata9905_primary_gap(19000U);
    emit_miata9905_secondary_gap(10000U);
    emit_miata9905_secondary_gap(10000U);
    emit_miata9905_secondary_gap(1000U);
    emit_miata9905_secondary_gap(1000U);

    TEST_ASSERT_EQUAL_UINT16(4U, testGetSecondaryToothCount());

    emit_miata9905_primary_gap(8000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());

    emit_miata9905_secondary_gap(10000U);
    emit_miata9905_secondary_gap(10000U);
    emit_miata9905_primary_gap(10000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(0U, testGetSecondaryToothCount());
}

static void test_miata9905_synced_cycle_wraps_after_tooth_eight(void)
{
    setup_miata9905_state_machine();

    emit_miata9905_primary_gap(19000U);
    emit_miata9905_primary_gap(30000U);
    emit_miata9905_secondary_gap(15000U);
    emit_miata9905_secondary_gap(5000U);
    emit_miata9905_primary_gap(4000U);

    TEST_ASSERT_EQUAL_UINT16(6U, toothCurrentCount);

    emit_miata9905_primary_gap(19000U);
    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(70U, triggerToothAngle);

    emit_miata9905_primary_gap(30000U);
    TEST_ASSERT_EQUAL_UINT16(8U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(110U, triggerToothAngle);

    emit_miata9905_primary_gap(19000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT16(1U, currentStatus.startRevolutions);
    TEST_ASSERT_EQUAL_UINT16(70U, triggerToothAngle);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothOneTime);
}

void testMiata9905()
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_miata9905_setup_seeds_unsynced_sentinel_and_filters);
        RUN_TEST_P(test_miata9905_two_cam_edges_sync_next_primary_tooth_six);
        RUN_TEST_P(test_miata9905_extra_cam_edges_defer_sync_until_clean_pair);
        RUN_TEST_P(test_miata9905_synced_cycle_wraps_after_tooth_eight);
    }
}
