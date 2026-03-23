#include <decoders.h>
#include <globals.h>
#include <unity.h>
#include "renix.h"
#include "schedule_calcs.h"
#include "../../test_utils.h"

extern volatile unsigned long toothLastToothTime;
extern volatile unsigned long toothLastMinusOneToothTime;
extern volatile unsigned long toothLastToothRisingTime;
extern volatile unsigned long toothLastSecToothRisingTime;
extern volatile uint16_t toothCurrentCount;
extern volatile bool revolutionOne;

static void reset_renix_runtime(void)
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
    configPage4.sparkMode = IGN_MODE_SEQUENTIAL;
    configPage4.triggerAngle = 0;
    configPage4.triggerFilter = TRIGGER_FILTER_LITE;

    toothLastToothTime = 0U;
    toothLastMinusOneToothTime = 0U;
    toothLastToothRisingTime = 0U;
    toothLastSecToothRisingTime = 0U;
    toothCurrentCount = 0U;
    revolutionOne = false;
    resetDecoder();
}

static void setup_renix44_state_machine(void)
{
    reset_renix_runtime();
    configPage4.TrigPattern = DECODER_RENIX;
    configPage2.nCylinders = 4U;
    triggerSetup_Renix();
}

static void emit_renix_primary_gap(unsigned long gapUs)
{
    delayMicroseconds(gapUs);
    triggerPri_Renix();
}

static void emit_renix_valid_group(unsigned long shortGapUs = 1000U, unsigned long longGapUs = 4000U)
{
    for (uint8_t tooth = 0; tooth < 8U; ++tooth)
    {
        emit_renix_primary_gap(shortGapUs);
    }
    emit_renix_primary_gap(longGapUs);
}

void test_setup_renix44()
{
    //Setup a renix 44 tooth wheel
    configPage4.TrigPattern = DECODER_RENIX;
    configPage2.nCylinders = 4;

    triggerSetup_Renix();
}

void test_setup_renix66()
{
    //Setup a renix 66 tooth wheel
    configPage4.TrigPattern = DECODER_RENIX;
    configPage2.nCylinders = 6;

    triggerSetup_Renix();
}

//************************************** Begin the new ignition setEndTooth tests **************************************
void test_Renix_newIgn_44_trig0_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=0
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(2, ignition1EndTooth);
}

void test_Renix_newIgn_44_trig90_1()
{
    //Test the set end tooth function. Conditions:

    //Advance: 10
    //triggerAngle=90
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(1, ignition1EndTooth);
}

void test_Renix_newIgn_44_trig180_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=180
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(4, ignition1EndTooth);
}

void test_Renix_newIgn_44_trig270_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=270
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition1EndTooth);
}

void test_Renix_newIgn_44_trig360_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=360
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition1EndTooth);
}

void test_Renix_newIgn_44_trigNeg90_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-90
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition1EndTooth);
}

void test_Renix_newIgn_44_trigNeg180_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-180
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(4, ignition1EndTooth);
}

void test_Renix_newIgn_44_trigNeg270_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-270
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(1, ignition1EndTooth);
}

void test_Renix_newIgn_44_trigNeg360_1()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-360
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition1EndAngle = 360 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(2, ignition1EndTooth);
}

// ******* CHannel 2 *******
void test_Renix_newIgn_44_trig0_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=0
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(4, ignition2EndTooth);
}

void test_Renix_newIgn_44_trig90_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=90
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 90; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition2EndTooth);
}

void test_Renix_newIgn_44_trig180_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=180
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 180; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition2EndTooth);
}

void test_Renix_newIgn_44_trig270_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=270
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 270; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(2, ignition2EndTooth);
}

void test_Renix_newIgn_44_trig366()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=360
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 360; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(1, ignition2EndTooth);
}

void test_Renix_newIgn_44_trigNeg90_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-90
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -90; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(1, ignition2EndTooth);
}

void test_Renix_newIgn_44_trigNeg180_2()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-180
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -180; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(2, ignition2EndTooth);
}

void test_Renix_newIgn_44_trigNeg270_2()
{
    //Test the set end tooth function. Conditions:
     //Advance: 10
    //triggerAngle=-270
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -270; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(3, ignition2EndTooth);
}

void test_Renix_newIgn_44_trigNeg366()
{
    //Test the set end tooth function. Conditions:
    //Advance: 10
    //triggerAngle=-360
    test_setup_renix44();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = -360; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(4, ignition2EndTooth);
}

void test_Renix_newIgn_66_trig0_2()
{
    //Test the set end tooth function. Conditions:

    //Advance: 10
    //triggerAngle=300
    test_setup_renix66();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 0; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(1, ignition2EndTooth);
}

void test_Renix_newIgn_66_trig181_2()
{
    //Test the set end tooth function. Conditions:

    //Advance: 10
    //triggerAngle=300
    test_setup_renix66();
    configPage4.sparkMode = IGN_MODE_SINGLE;
    ignition2EndAngle = 180 - 10; //Set 10 degrees advance
    configPage4.triggerAngle = 181; //No trigger offset
    
    triggerSetEndTeeth_Renix();
    TEST_ASSERT_EQUAL(5, ignition2EndTooth);
}

static void test_renix_state_valid_gap_advances_virtual_tooth_without_sync_loss(void)
{
    setup_renix44_state_machine();

    emit_renix_valid_group();

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
    TEST_ASSERT_NOT_EQUAL_UINT32(0U, toothLastToothTime);
}

static void test_renix_state_short_gap_is_filtered_after_valid_group(void)
{
    setup_renix44_state_machine();

    emit_renix_valid_group();
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);

    emit_renix_primary_gap(50U);

    TEST_ASSERT_FALSE(currentStatus.hasSync);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.syncLossCounter);
    TEST_ASSERT_EQUAL_UINT16(2U, toothCurrentCount);
}

void testRenix()
{
  SET_UNITY_FILENAME() {

  RUN_TEST_P(test_Renix_newIgn_44_trig0_1);
  RUN_TEST_P(test_Renix_newIgn_44_trig90_1);
  RUN_TEST_P(test_Renix_newIgn_44_trig180_1);
  RUN_TEST_P(test_Renix_newIgn_44_trig270_1);
  RUN_TEST_P(test_Renix_newIgn_44_trig360_1);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg90_1);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg180_1);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg270_1);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg360_1);

  RUN_TEST_P(test_Renix_newIgn_44_trig0_2);
  RUN_TEST_P(test_Renix_newIgn_44_trig90_2);
  RUN_TEST_P(test_Renix_newIgn_44_trig180_2);
  RUN_TEST_P(test_Renix_newIgn_44_trig270_2);
  RUN_TEST_P(test_Renix_newIgn_44_trig366);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg90_2);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg180_2);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg270_2);
  RUN_TEST_P(test_Renix_newIgn_44_trigNeg366);

  RUN_TEST_P(test_Renix_newIgn_66_trig0_2);
  RUN_TEST_P(test_Renix_newIgn_66_trig181_2);
  RUN_TEST_P(test_renix_state_valid_gap_advances_virtual_tooth_without_sync_loss);
  RUN_TEST_P(test_renix_state_short_gap_is_filtered_after_valid_group);
  }           
}
