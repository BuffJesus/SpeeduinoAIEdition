#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "FordST170.h"
#include "scheduler.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern uint16_t ignition1EndTooth;
extern uint16_t ignition2EndTooth;
extern uint16_t ignition3EndTooth;
extern uint16_t ignition4EndTooth;
extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;
extern volatile unsigned long triggerSecFilterTime;

static void reset_fordst170_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage2.strokes = FOUR_STROKE;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.perToothIgn = false;
    configPage6.vvtEnabled = 0U;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_fordst170_state_machine(void)
{
    reset_fordst170_runtime();
    triggerSetup_FordST170();
}

static void emit_fordst170_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_missingTooth();
}

static void emit_fordst170_missing_tooth_cycle(unsigned long normalGapUs = 2000U, unsigned long missingGapUs = 4000U)
{
    emit_fordst170_primary_gap(normalGapUs);
    for (uint8_t tooth = 0; tooth < 34U; ++tooth)
    {
        emit_fordst170_primary_gap(normalGapUs);
    }
    emit_fordst170_primary_gap(missingGapUs);
}

static void emit_fordst170_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_FordST170();
}

static void test_fordst170_state_short_cam_pulse_is_filtered_without_regrouping(void)
{
    setup_fordst170_state_machine();

    emit_fordst170_secondary_gap(2000U);
    emit_fordst170_secondary_gap(2000U);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    emit_fordst170_secondary_gap(50U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, testGetSecondaryToothCount());
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    emit_fordst170_secondary_gap(2000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(2U, testGetSecondaryToothCount());
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);
}

void test_fordst170_newIgn_12_trig0_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=0
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);

    //Test again with 0 degrees advance
    calculateIgnitionAngle(5, 0, 0, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(35, ignition1EndTooth);

    //Test again with 35 degrees advance
    calculateIgnitionAngle(5, 0, 35, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(31, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trig90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=90
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 90; //No trigger offset
    calculateIgnitionAngle(5, 0, 35, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(22, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trig180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 180; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
 
    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trig270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 270; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trig360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 360; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    
    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trigNeg90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -90; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trigNeg180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -180; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trigNeg270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -270; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    
    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(25, ignition1EndTooth);
}

void test_fordst170_newIgn_12_trigNeg360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    triggerSetup_FordST170();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -360; //No trigger offset
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 

    triggerSetEndTeeth_FordST170();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

void test_fordst170_state_primary_only_sets_half_sync(void)
{
    setup_fordst170_state_machine();

    emit_fordst170_missing_tooth_cycle();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void test_fordst170_state_cam_activity_before_gap_establishes_full_sync(void)
{
    setup_fordst170_state_machine();

    emit_fordst170_secondary_gap(2000U);
    emit_fordst170_secondary_gap(2000U);
    emit_fordst170_missing_tooth_cycle();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void test_fordst170_state_sixth_cam_tooth_restarts_grouping(void)
{
    setup_fordst170_state_machine();

    emit_fordst170_secondary_gap(2000U);
    emit_fordst170_secondary_gap(2000U);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    for (uint8_t tooth = 0; tooth < 4U; ++tooth)
    {
        emit_fordst170_secondary_gap(1000U);
    }
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_TRUE(triggerSecFilterTime > 0U);

    emit_fordst170_secondary_gap(1000U);

    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerSecFilterTime);

    emit_fordst170_missing_tooth_cycle();
    TEST_ASSERT_TRUE(currentStatus.hasSync);
}

void testFordST170()
{
    SET_UNITY_FILENAME() {

    RUN_TEST_P(test_fordst170_newIgn_12_trig0_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trig90_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trig180_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trig270_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trig360_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trigNeg90_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trigNeg180_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trigNeg270_1);
    RUN_TEST_P(test_fordst170_newIgn_12_trigNeg360_1);
    RUN_TEST_P(test_fordst170_state_primary_only_sets_half_sync);
    RUN_TEST_P(test_fordst170_state_cam_activity_before_gap_establishes_full_sync);
    RUN_TEST_P(test_fordst170_state_short_cam_pulse_is_filtered_without_regrouping);
    RUN_TEST_P(test_fordst170_state_sixth_cam_tooth_restarts_grouping);
    
    }
}
