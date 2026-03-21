#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "missing_tooth.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

void test_setup_36_1()
{
    //Setup a 36-1 wheel
    configPage4.triggerTeeth = 36;
    configPage4.triggerMissingTeeth = 1;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;

    triggerSetup_missingTooth();
}

void test_setup_60_2()
{
    //Setup a 60-2 wheel
    configPage4.triggerTeeth = 60;
    configPage4.triggerMissingTeeth = 2;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;

    triggerSetup_missingTooth();
}

static void reset_missing_tooth_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    configPage6.vvtEnabled = 0U;
    configPage2.strokes = FOUR_STROKE;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothCurrentCount = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void emit_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_missingTooth();
}

static void emit_missing_tooth_cycle_36_1(unsigned long normalGapUs = 2000U, unsigned long missingGapUs = 4000U)
{
    emit_primary_gap(normalGapUs);
    for (uint8_t tooth = 0; tooth < 34U; tooth++)
    {
        emit_primary_gap(normalGapUs);
    }
    emit_primary_gap(missingGapUs);
}

//************************************** Begin the new ignition setEndTooth tests **************************************
void test_missingtooth_newIgn_36_1_trig0_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=0
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trig90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=90
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(25, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trig180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trig270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trig360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(7, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(16, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(25, ignition1EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(34, ignition1EndTooth);
}

// ******* CHannel 2 *******
void test_missingtooth_newIgn_36_1_trig0_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=0
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trig90_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=90
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(7, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trig180_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(34, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trig270_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(25, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trig360_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg90_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(25, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg180_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(34, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg270_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(7, ignition2EndTooth);
}

void test_missingtooth_newIgn_36_1_trigNeg360_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    test_setup_36_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_missingtooth_newIgn_60_2_trig0_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 60-2
    //Advance: 10
    //triggerAngle=300
    test_setup_60_2();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(57, ignition2EndTooth);
}

void test_missingtooth_newIgn_60_2_trig181_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 60-2
    //Advance: 10
    //triggerAngle=300
    test_setup_60_2();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 181; //No trigger offset
    
    triggerSetEndTeeth_missingTooth();
    TEST_ASSERT_EQUAL(58, ignition2EndTooth);
}


void test_missingtooth_newIgn_2()
{

}
void test_missingtooth_newIgn_3()
{

}
void test_missingtooth_newIgn_4()
{

}

static void test_missingtooth_state_sync_acquired_on_gap_after_full_cycle(void)
{
    test_setup_36_1();
    reset_missing_tooth_runtime();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;

    emit_missing_tooth_cycle_36_1();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.startRevolutions);
}

static void test_missingtooth_state_sequential_requires_cam_before_full_sync(void)
{
    test_setup_36_1();
    reset_missing_tooth_runtime();
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage2.injLayout = INJ_SEQUENTIAL;

    emit_missing_tooth_cycle_36_1();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);

    triggerSec_missingTooth();
    delayMicroseconds(10000U);
    triggerSec_missingTooth();

    emit_missing_tooth_cycle_36_1();

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_TRUE(currentStatus.startRevolutions > 0U);
}

static void test_missingtooth_state_sync_loss_when_gap_arrives_too_early(void)
{
    test_setup_36_1();
    reset_missing_tooth_runtime();
    configPage4.sparkMode = IGN_MODE_WASTED;
    configPage2.injLayout = INJ_SEMISEQUENTIAL;

    emit_missing_tooth_cycle_36_1();
    TEST_ASSERT_TRUE(currentStatus.hasSync);

    for (uint8_t tooth = 0; tooth < 10U; tooth++)
    {
        emit_primary_gap(2000U);
    }
    emit_primary_gap(4000U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_BIT_LOW(BIT_STATUS3_HALFSYNC, currentStatus.status3);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
}

void testMissingTooth()
{
   SET_UNITY_FILENAME() {

  RUN_TEST(test_missingtooth_newIgn_36_1_trig0_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig90_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig180_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig270_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig360_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg90_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg180_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg270_1);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg360_1);

  RUN_TEST(test_missingtooth_newIgn_36_1_trig0_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig90_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig180_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig270_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trig360_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg90_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg180_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg270_2);
  RUN_TEST(test_missingtooth_newIgn_36_1_trigNeg360_2);

  RUN_TEST(test_missingtooth_state_sync_acquired_on_gap_after_full_cycle);
  RUN_TEST(test_missingtooth_state_sequential_requires_cam_before_full_sync);
  RUN_TEST(test_missingtooth_state_sync_loss_when_gap_arrives_too_early);

  //RUN_TEST(test_missingtooth_newIgn_60_2_trig181_2);
  //RUN_TEST(test_missingtooth_newIgn_60_2_trig182_2);
   }
}
