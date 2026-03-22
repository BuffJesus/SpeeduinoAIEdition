#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "dual_wheel.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern volatile unsigned long toothLastToothTime;
static void reset_dualwheel_runtime(void)
{
    currentStatus.hasSync = false;
    currentStatus.syncLossCounter = 0U;
    currentStatus.startRevolutions = 0U;
    currentStatus.status3 = 0U;
    currentStatus.RPM = 0U;
    currentStatus.crankRPM = 400U;
    currentStatus.engine = 0U;
    configPage4.StgCycles = 0U;
    configPage4.useResync = 0U;
    configPage4.triggerAngle = 0;
    configPage2.perToothIgn = false;
    configPage2.strokes = FOUR_STROKE;
    revolutionOne = false;
    resetDecoder();
}

static void setup_dualwheel_state_machine(void)
{
    configPage4.triggerTeeth = 12U;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage2.injLayout = INJ_SEQUENTIAL;
    triggerSetup_DualWheel();
    reset_dualwheel_runtime();
}

static void emit_dualwheel_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_DualWheel();
}

static void emit_dualwheel_secondary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerSec_DualWheel();
}


void test_setup_dualwheel_12_1()
{
    //Setup a 36-1 wheel
    configPage4.triggerTeeth = 12;
    //configPage4.triggerMissingTeeth = 1;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;

    triggerSetup_missingTooth();
}

void test_setup_dualwheel_60_2()
{
    //Setup a 60-2 wheel
    configPage4.triggerTeeth = 60;
    configPage4.triggerMissingTeeth = 2;
    configPage4.TrigSpeed = CRANK_SPEED;
    configPage4.trigPatternSec = SEC_TRIGGER_SINGLE;

    triggerSetup_missingTooth();
}

//************************************** Begin the new ignition setEndTooth tests **************************************
void test_dualwheel_newIgn_12_1_trig0_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=0
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(11, ignition1EndTooth);

    //Test again with 0 degrees advance
    ignition1EndAngle = 360 - 0; //Set 0 degrees advance
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(12, ignition1EndTooth);

    //Test again with 35 degrees advance
    ignition1EndAngle = 360 - 35; //Set 35 degrees advance
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(10, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trig90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 12/1
    //Advance: 10
    //triggerAngle=90
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(8, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trig180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(5, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trig270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(2, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trig360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(12, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg90_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(2, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg180_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(5, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg270_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(8, ignition1EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg360_1()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(11, ignition1EndTooth);
}

// ******* CHannel 2 *******
void test_dualwheel_newIgn_12_1_trig0_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=0
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trig90_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=90
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(7, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trig180_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=180
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(34, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trig270_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=270
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(25, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trig360_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=360
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg90_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-90
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(25, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg180_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-180
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(34, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg270_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-270
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(7, ignition2EndTooth);
}

void test_dualwheel_newIgn_12_1_trigNeg360_2()
{
    //Test the set end tooth function. Conditions:
    //Trigger: 36-1
    //Advance: 10
    //triggerAngle=-360
    test_setup_dualwheel_12_1();
    configPage4.sparkMode = IGN_MODE_WASTED;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_DualWheel();
    TEST_ASSERT_EQUAL(16, ignition2EndTooth);
}

void test_dualwheel_newIgn_2()
{

}
void test_dualwheel_newIgn_3()
{

}
void test_dualwheel_newIgn_4()
{

}

static void test_dualwheel_state_secondary_pulse_establishes_sync_and_primes_primary(void)
{
    setup_dualwheel_state_machine();

    emit_dualwheel_secondary_gap(6000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT32(0U, triggerFilterTime);
}

static void test_dualwheel_state_first_primary_after_sync_wraps_and_counts_revolution(void)
{
    setup_dualwheel_state_machine();
    emit_dualwheel_secondary_gap(6000U);

    emit_dualwheel_primary_gap(3000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(1U, toothCurrentCount);
    TEST_ASSERT_FALSE(revolutionOne);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.startRevolutions);
    TEST_ASSERT_TRUE(triggerFilterTime >= 750U);
    TEST_ASSERT_TRUE(triggerFilterTime < 1000U);
}

static void test_dualwheel_state_short_secondary_gap_is_filtered(void)
{
    setup_dualwheel_state_machine();
    revolutionTime = 8000U;
    emit_dualwheel_secondary_gap(6000U);
    triggerSecFilterTime = 5000U;

    emit_dualwheel_secondary_gap(1000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_EQUAL_UINT32(revolutionTime >> 1, triggerSecFilterTime);
}

static void test_dualwheel_state_resync_counts_sync_loss_and_resets_tooth(void)
{
    setup_dualwheel_state_machine();
    configPage4.useResync = 1U;
    currentStatus.hasSync = true;
    currentStatus.startRevolutions = 3U;
    toothCurrentCount = 3U;
    triggerSecFilterTime = 1000U;

    emit_dualwheel_secondary_gap(6000U);

    TEST_ASSERT_TRUE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(1U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(configPage4.triggerTeeth, toothCurrentCount);
    TEST_ASSERT_TRUE(revolutionOne);
}

void testDualWheel()
{
  SET_UNITY_FILENAME() {

  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig0_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig90_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig180_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig270_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig360_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg90_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg180_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg270_1);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg360_1);
  RUN_TEST_P(test_dualwheel_state_secondary_pulse_establishes_sync_and_primes_primary);
  RUN_TEST_P(test_dualwheel_state_first_primary_after_sync_wraps_and_counts_revolution);
  RUN_TEST_P(test_dualwheel_state_short_secondary_gap_is_filtered);
  RUN_TEST_P(test_dualwheel_state_resync_counts_sync_loss_and_resets_tooth);

/*
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig0_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig90_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig180_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig270_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trig360_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg90_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg180_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg270_2);
  RUN_TEST_P(test_dualwheel_newIgn_12_1_trigNeg360_2);
*/
  //RUN_TEST(test_dualwheel_newIgn_60_2_trig181_2);
  //RUN_TEST(test_dualwheel_newIgn_60_2_trig182_2);

  }
}
