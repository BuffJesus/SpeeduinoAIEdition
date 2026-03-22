#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "test_ngc.h"
#include "scheduler.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern uint16_t ignition1EndTooth;
extern uint16_t ignition2EndTooth;
extern uint16_t ignition3EndTooth;
extern uint16_t ignition4EndTooth;
extern volatile unsigned long toothLastToothRisingTime;
extern volatile unsigned long toothLastSecToothRisingTime;

static void reset_ngc_runtime(void)
{
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
}

static void setup_ngc4_state_machine(void)
{
    reset_ngc_runtime();
    triggerSetup_NGC();
    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinTrigger2, LOW);
}

static void emit_ngc4_primary_tooth(unsigned long lowUs = 300U, unsigned long highUs = 1200U)
{
    delayMicroseconds(lowUs);
    toothLastToothRisingTime = micros();
    delayMicroseconds(highUs);
    digitalWrite(pinTrigger, LOW);
    triggerPri_NGC();
}

static void emit_ngc4_primary_missing_tooth_high(void)
{
    delayMicroseconds(600U);
    toothLastToothRisingTime = micros();
    delayMicroseconds(4200U);
    digitalWrite(pinTrigger, LOW);
    triggerPri_NGC();
}

void test_ngc_newIgn_12_trig0_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 0; //No trigger offset

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);    
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);

    //Test again with 0 degrees advance
    calculateIgnitionAngle(5, 0, 0, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);

    //Test again with 35 degrees advance
    calculateIgnitionAngle(5, 0, 35, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(31, ignition1EndTooth);
}

void test_ngc_newIgn_12_trig90_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 90;

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);    
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(25, ignition1EndTooth);
}

void test_ngc_newIgn_12_trig180_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 180;

    currentStatus.advance = 10; //Set 10deg advance
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 

    
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_ngc_newIgn_12_trig270_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 270;

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);     
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_ngc_newIgn_12_trig360_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = 360;

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);     
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

void test_ngc_newIgn_12_trigNeg90_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -90;
    
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle); 
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_ngc_newIgn_12_trigNeg180_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -180;

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);   
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_ngc_newIgn_12_trigNeg270_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -270;

    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);   
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(25, ignition1EndTooth);
}

void test_ngc_newIgn_12_trigNeg360_1()
{
    triggerSetup_NGC();
    CRANK_ANGLE_MAX_IGN = 360;
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage4.triggerAngle = -360;
    
    calculateIgnitionAngle(5, 0, 10, &ignition1EndAngle, &ignition1StartAngle);     
    triggerSetEndTeeth_NGC();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

static void emit_ngc4_first_missing_tooth_without_cam(void)
{
    emit_ngc4_primary_tooth();
    emit_ngc4_primary_tooth();
    emit_ngc4_primary_tooth();
    emit_ngc4_primary_missing_tooth_high();
}

void test_ngc4_state_primary_only_sets_half_sync(void)
{
    setup_ngc4_state_machine();

    emit_ngc4_first_missing_tooth_without_cam();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void test_ngc4_state_valid_cam_phase_establishes_full_sync(void)
{
    setup_ngc4_state_machine();

    testSetSecondaryToothCount(1U);
    emit_ngc4_first_missing_tooth_without_cam();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void test_ngc4_state_invalid_cam_phase_drops_back_to_half_sync(void)
{
    setup_ngc4_state_machine();

    testSetSecondaryToothCount(5U);
    emit_ngc4_first_missing_tooth_without_cam();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status3, BIT_STATUS3_HALFSYNC));
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
}

void testNGC()
{
   SET_UNITY_FILENAME() {

    RUN_TEST_P(test_ngc_newIgn_12_trig0_1);
    RUN_TEST_P(test_ngc_newIgn_12_trig90_1);
    RUN_TEST_P(test_ngc_newIgn_12_trig180_1);
    RUN_TEST_P(test_ngc_newIgn_12_trig270_1);
    RUN_TEST_P(test_ngc_newIgn_12_trig360_1);
    RUN_TEST_P(test_ngc_newIgn_12_trigNeg90_1);
    RUN_TEST_P(test_ngc_newIgn_12_trigNeg180_1);
    RUN_TEST_P(test_ngc_newIgn_12_trigNeg270_1);
    RUN_TEST_P(test_ngc_newIgn_12_trigNeg360_1);
    RUN_TEST_P(test_ngc4_state_primary_only_sets_half_sync);
    RUN_TEST_P(test_ngc4_state_valid_cam_phase_establishes_full_sync);
    RUN_TEST_P(test_ngc4_state_invalid_cam_phase_drops_back_to_half_sync);
   }
}
