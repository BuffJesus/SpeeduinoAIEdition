#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "Nissan360.h"
#include "scheduler.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern uint16_t ignition1EndTooth;
extern uint16_t ignition2EndTooth;
extern uint16_t ignition3EndTooth;
extern uint16_t ignition4EndTooth;
extern volatile unsigned long toothLastToothTime;

static void reset_nissan360_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage4.useResync = 0U;
    configPage4.triggerAngle = 0;
    configPage4.TrigEdgeSec = 0U;
    configPage2.nCylinders = 4U;
    configPage2.perToothIgn = false;
    resetDecoder();
}

static void setup_nissan360_state_machine(uint8_t cylinderCount = 4U)
{
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage2.injLayout = INJ_SEQUENTIAL;
    configPage2.nCylinders = cylinderCount;
    triggerSetup_Nissan360();
    reset_nissan360_runtime();
    configPage2.nCylinders = cylinderCount;
}

static void emit_nissan360_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Nissan360();
}

static void emit_nissan360_secondary_window_start(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    configPage4.TrigEdgeSec = 0U;
    triggerSec_Nissan360();
}

static void emit_nissan360_secondary_window_end(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    configPage4.TrigEdgeSec = 1U;
    triggerSec_Nissan360();
}

static void emit_nissan360_window(unsigned long edgeGapUs, uint16_t primaryTeeth)
{
    emit_nissan360_secondary_window_start(edgeGapUs);
    for (uint16_t tooth = 0; tooth < primaryTeeth; ++tooth)
    {
        emit_nissan360_primary_gap(1000U);
    }
    emit_nissan360_secondary_window_end(edgeGapUs);
}

void test_nissan360_newIgn_12_trig0_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=0
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);    
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(171, ignition1EndTooth);

    //Test again with 0 degrees advance
    calculateIgnitionAngle(5, 0, 0, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(176, ignition1EndTooth);

    //Test again with 35 degrees advance
    calculateIgnitionAngle(5, 0, 35, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(158, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trig90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=90
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 90; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);    
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(126, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trig180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 180; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);    
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(81, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trig270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 270; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);  
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(36, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trig360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 360; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(351, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trigNeg90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -90; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(216, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trigNeg180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -180; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(261, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trigNeg270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -270; //No trigger offset
    
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(306, ignition1EndTooth);
}

void test_nissan360_newIgn_12_trigNeg360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    triggerSetup_Nissan360();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -360; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_Nissan360();
    TEST_ASSERT_EQUAL(351, ignition1EndTooth);
}

void test_nissan360_state_syncs_on_valid_long_window(void)
{
    setup_nissan360_state_machine();

    emit_nissan360_window(2000U, 16U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);
}

void test_nissan360_state_other_supported_cylinder_windows_sync(void)
{
    setup_nissan360_state_machine(6U);
    emit_nissan360_window(2000U, 4U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(124U, toothCurrentCount);

    setup_nissan360_state_machine(8U);
    emit_nissan360_window(2000U, 7U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(56U, toothCurrentCount);
}

void test_nissan360_state_syncs_on_other_valid_4cyl_windows(void)
{
    setup_nissan360_state_machine();
    emit_nissan360_window(2000U, 12U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(102U, toothCurrentCount);

    setup_nissan360_state_machine();
    emit_nissan360_window(2000U, 8U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(188U, toothCurrentCount);

    setup_nissan360_state_machine();
    emit_nissan360_window(2000U, 4U);
    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(274U, toothCurrentCount);
}

void test_nissan360_state_invalid_window_does_not_sync(void)
{
    setup_nissan360_state_machine();

    emit_nissan360_window(2000U, 6U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(7U, toothCurrentCount);
}

void test_nissan360_state_wraps_and_counts_revolution_when_synced(void)
{
    setup_nissan360_state_machine();

    emit_nissan360_window(2000U, 16U);

    for (uint16_t tooth = 0; tooth < 345U; ++tooth)
    {
        emit_nissan360_primary_gap(1000U);
    }

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

void test_nissan360_state_resync_realigns_tooth_counter(void)
{
    setup_nissan360_state_machine();

    currentStatus.hasSync = true;
    configPage4.useResync = true;
    toothCurrentCount = 50U;

    emit_nissan360_window(2000U, 16U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(16U, toothCurrentCount);
}

void testNissan360()
{
  SET_UNITY_FILENAME() {


    RUN_TEST_P(test_nissan360_newIgn_12_trig0_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trig90_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trig180_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trig270_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trig360_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trigNeg90_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trigNeg180_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trigNeg270_1);
    RUN_TEST_P(test_nissan360_newIgn_12_trigNeg360_1);
    RUN_TEST_P(test_nissan360_state_syncs_on_valid_long_window);
    RUN_TEST_P(test_nissan360_state_syncs_on_other_valid_4cyl_windows);
    RUN_TEST_P(test_nissan360_state_other_supported_cylinder_windows_sync);
    RUN_TEST_P(test_nissan360_state_invalid_window_does_not_sync);
    RUN_TEST_P(test_nissan360_state_wraps_and_counts_revolution_when_synced);
    RUN_TEST_P(test_nissan360_state_resync_realigns_tooth_counter);
  }
}
