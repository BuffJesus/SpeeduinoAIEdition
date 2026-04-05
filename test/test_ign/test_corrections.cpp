#include <unity.h>
#include "globals.h"
#include "corrections.h"
#include "knock.h"
// #include "init.h"
#include "engineProtection.h"
#include "idle.h"
#include "comms_legacy.h"
#include "comms_secondary.h"
#include "logger.h"
#include "speeduino.h"
#include "../test_utils.h"
#include "sensors.h"
#include <string.h>

extern void construct2dTables(void);

extern int8_t correctionFixedTiming(int8_t advance);

static void test_correctionFixedTiming_inactive(void) {
    configPage2.fixAngEnable = 0;
    configPage4.FixAng = 13;

    TEST_ASSERT_EQUAL(8, correctionFixedTiming(8));
    TEST_ASSERT_EQUAL(-3, correctionFixedTiming(-3));
}

static void test_correctionFixedTiming_active(void) {
    configPage2.fixAngEnable = 1;
    configPage4.FixAng = 13;

    TEST_ASSERT_EQUAL(configPage4.FixAng, correctionFixedTiming(8));
    TEST_ASSERT_EQUAL(configPage4.FixAng, correctionFixedTiming(-3));
}

static void test_correctionFixedTiming(void) {
    RUN_TEST_P(test_correctionFixedTiming_inactive);
    RUN_TEST_P(test_correctionFixedTiming_active);
}

extern int8_t correctionCLTadvance(int8_t advance);

static void setup_clt_advance_table(void) {
  construct2dTables();
  initialiseCorrections();
  TEST_DATA_P uint8_t bins[] = { 60, 70, 80, 90, 100, 110 };
  TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
  populate_2dtable_P(&CLTAdvanceTable, values, bins);
}

static void test_correctionCLTadvance_lookup(void) {
    setup_clt_advance_table();

    currentStatus.coolant = 105 - CALIBRATION_TEMPERATURE_OFFSET;
    TEST_ASSERT_EQUAL(8 + 8 - 15, correctionCLTadvance(8));

    currentStatus.coolant = 65 - CALIBRATION_TEMPERATURE_OFFSET;
    TEST_ASSERT_EQUAL(1 + 28 - 15, correctionCLTadvance(1));

    currentStatus.coolant = 105 - CALIBRATION_TEMPERATURE_OFFSET;
    TEST_ASSERT_EQUAL(-3 + 8 - 15, correctionCLTadvance(-3));
}

static void test_correctionCLTadvance(void) {
    RUN_TEST_P(test_correctionCLTadvance_lookup);
}

static void test_correctionCrankingFixedTiming_nocrank_inactive(void) {
    setup_clt_advance_table();
    BIT_CLEAR(currentStatus.engine, BIT_ENGINE_CRANK);
    configPage2.crkngAddCLTAdv = 0;
    configPage4.CrankAng = 8;

    TEST_ASSERT_EQUAL(-7, correctionCrankingFixedTiming(-7));
}

static void test_correctionCrankingFixedTiming_crank_fixed(void) {
    setup_clt_advance_table();
    BIT_SET(currentStatus.engine, BIT_ENGINE_CRANK);
    configPage2.crkngAddCLTAdv = 0;

    configPage4.CrankAng = 8;
    TEST_ASSERT_EQUAL(configPage4.CrankAng, correctionCrankingFixedTiming(-7));

    configPage4.CrankAng = -8;
    TEST_ASSERT_EQUAL(configPage4.CrankAng, correctionCrankingFixedTiming(-7));
}

static void test_correctionCrankingFixedTiming_crank_coolant(void) {
    setup_clt_advance_table();
    BIT_SET(currentStatus.engine, BIT_ENGINE_CRANK);
    configPage2.crkngAddCLTAdv = 1;
    
    configPage4.CrankAng = 8;

    currentStatus.coolant = 65 - CALIBRATION_TEMPERATURE_OFFSET;
    TEST_ASSERT_EQUAL(1 + 28 - 15, correctionCLTadvance(1));
}

static void test_correctionCrankingFixedTiming(void) {
    RUN_TEST_P(test_correctionCrankingFixedTiming_nocrank_inactive);
    RUN_TEST_P(test_correctionCrankingFixedTiming_crank_fixed);
    RUN_TEST_P(test_correctionCrankingFixedTiming_crank_coolant);
}

extern int8_t correctionFlexTiming(int8_t advance);

static void setup_flexAdv(void) {
  construct2dTables();
  initialiseCorrections();
  TEST_DATA_P uint8_t bins[] = { 30, 40, 50, 60, 70, 80 };
  TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
  populate_2dtable_P(&flexAdvTable, values, bins);

  configPage2.flexEnabled = 1;
  currentStatus.ethanolPct = 55;
}

static void test_correctionFlexTiming_inactive(void) {
    setup_flexAdv();
    configPage2.flexEnabled = 0;

    TEST_ASSERT_EQUAL(-7, correctionFlexTiming(-7));
    TEST_ASSERT_EQUAL(3, correctionFlexTiming(3));
}

static void test_correctionFlexTiming_table_lookup(void) {
    setup_flexAdv();

    TEST_ASSERT_EQUAL(8 + 18 - OFFSET_IGNITION, correctionFlexTiming(8));
    TEST_ASSERT_EQUAL(18 - OFFSET_IGNITION, currentStatus.flexIgnCorrection);    

    currentStatus.ethanolPct = 35;
    TEST_ASSERT_EQUAL(-4 + 28 - OFFSET_IGNITION, correctionFlexTiming(-4));
    TEST_ASSERT_EQUAL(28 - OFFSET_IGNITION, currentStatus.flexIgnCorrection);    
}

static void test_correctionFlexTiming(void) {
    RUN_TEST_P(test_correctionFlexTiming_inactive);
    RUN_TEST_P(test_correctionFlexTiming_table_lookup);
}

extern int8_t correctionWMITiming(int8_t advance);

static void setup_WMIAdv(void) {
    construct2dTables();
    initialiseCorrections();

    configPage10.wmiEnabled= 1;
    configPage10.wmiAdvEnabled = 1;
    BIT_CLEAR(currentStatus.status4, BIT_STATUS4_WMI_EMPTY);
    configPage10.wmiTPS = 50;
    currentStatus.TPS = configPage10.wmiTPS + 1;
    configPage10.wmiRPM = 30;
    currentStatus.RPM = configPage10.wmiRPM + 1U;
    configPage10.wmiMAP = 35;
    currentStatus.MAP = (configPage10.wmiMAP*2L)+1L;
    configPage10.wmiIAT = 155;
    currentStatus.IAT = configPage10.wmiIAT - CALIBRATION_TEMPERATURE_OFFSET + 1;

    TEST_DATA_P uint8_t bins[] = { 30, 40, 50, 60, 70, 80 };
    TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
    populate_2dtable_P(&wmiAdvTable, values, bins);
}

static void test_correctionWMITiming_table_lookup(void) {
    setup_WMIAdv();

    currentStatus.MAP = (55*2U)+1U;
    TEST_ASSERT_EQUAL(8 + 18 - OFFSET_IGNITION, correctionWMITiming(8));

    currentStatus.MAP = (35*2U)+1U;
    TEST_ASSERT_EQUAL(-4 + 28 - OFFSET_IGNITION, correctionWMITiming(-4));
}

static void test_correctionWMITiming_wmidisabled_inactive(void) {
    setup_WMIAdv();
    configPage10.wmiEnabled= 0;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}


static void test_correctionWMITiming_wmiadvdisabled_inactive(void) {
    setup_WMIAdv();
    configPage10.wmiAdvEnabled = 0;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}

static void test_correctionWMITiming_empty_inactive(void) {
    setup_WMIAdv();
    BIT_SET(currentStatus.status4, BIT_STATUS4_WMI_EMPTY);

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}

static void test_correctionWMITiming_tpslow_inactive(void) {
    setup_WMIAdv();
    currentStatus.TPS = configPage10.wmiTPS - 1;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}

static void test_correctionWMITiming_rpmlow_inactive(void) {
    setup_WMIAdv();
    currentStatus.RPM = configPage10.wmiRPM - 1U;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}
   
static void test_correctionWMITiming_maplow_inactive(void) {
    setup_WMIAdv();
    currentStatus.MAP = (configPage10.wmiMAP*2)-1;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}
    
static void test_correctionWMITiming_iatlow_inactive(void) {
    setup_WMIAdv();
    currentStatus.IAT = configPage10.wmiIAT - CALIBRATION_TEMPERATURE_OFFSET - 1;

    TEST_ASSERT_EQUAL(8, correctionWMITiming(8));
    TEST_ASSERT_EQUAL(-3, correctionWMITiming(-3));
}   

static void test_correctionWMITiming(void) {
    RUN_TEST_P(test_correctionWMITiming_table_lookup);
    RUN_TEST_P(test_correctionWMITiming_tpslow_inactive);
    RUN_TEST_P(test_correctionWMITiming_wmidisabled_inactive);
    RUN_TEST_P(test_correctionWMITiming_wmiadvdisabled_inactive);
    RUN_TEST_P(test_correctionWMITiming_empty_inactive);
    RUN_TEST_P(test_correctionWMITiming_tpslow_inactive);
    RUN_TEST_P(test_correctionWMITiming_rpmlow_inactive);
    RUN_TEST_P(test_correctionWMITiming_maplow_inactive);
    RUN_TEST_P(test_correctionWMITiming_iatlow_inactive);
}

extern int8_t correctionIATretard(int8_t advance);

static void setup_IATRetard(void) {
  construct2dTables();
  initialiseCorrections();

  TEST_DATA_P uint8_t bins[] = { 30, 40, 50, 60, 70, 80 };
  TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
  populate_2dtable_P(&IATRetardTable, values, bins);

  currentStatus.IAT = 75;
}

static void test_correctionIATretard_table_lookup(void) {
    setup_IATRetard();

    TEST_ASSERT_EQUAL(-11-8, correctionIATretard(-11));

    currentStatus.IAT = 35;
    TEST_ASSERT_EQUAL(11-28, correctionIATretard(11));
}

static void test_correctionIATretard(void) {
    RUN_TEST_P(test_correctionIATretard_table_lookup);
}

extern int8_t correctionIdleAdvance(int8_t advance);

static void setup_idleadv_tps(void) {
    configPage2.idleAdvAlgorithm = IDLEADVANCE_ALGO_TPS;
    configPage2.idleAdvTPS = 30;
    currentStatus.TPS = configPage2.idleAdvTPS - 1U;
}

static void setup_idleadv_ctps(void) {
    configPage2.idleAdvAlgorithm = IDLEADVANCE_ALGO_CTPS;
    currentStatus.CTPSActive = 1;
}

static void setup_correctionIdleAdvance(void) {
    construct2dTables();
    initialiseCorrections();

    TEST_DATA_P uint8_t bins[] = { 30, 40, 50, 60, 70, 80 };
    TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
    populate_2dtable_P(&idleAdvanceTable, values, bins);
  
    configPage2.idleAdvEnabled = IDLEADVANCE_MODE_ADDED;
    configPage2.idleAdvDelay = 5;
    configPage2.idleAdvRPM = 20;
    configPage2.vssMode = 0;
    configPage6.iacAlgorithm = IAC_ALGORITHM_NONE;
    configPage9.idleAdvStartDelay = 0U;

    runSecsX10 = configPage2.idleAdvDelay * 5;
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    // int idleRPMdelta = (currentStatus.CLIdleTarget - (currentStatus.RPM / 10) ) + 50;
    currentStatus.CLIdleTarget = 100;
    currentStatus.RPM = (configPage2.idleAdvRPM * 100) - 1U;
    
    setup_idleadv_tps();
    // Run once to initialise internal state
    correctionIdleAdvance(8);
}

static void assert_correctionIdleAdvance(int8_t advance, uint8_t expectedLookupValue) {
    configPage2.idleAdvEnabled = IDLEADVANCE_MODE_ADDED;
    TEST_ASSERT_EQUAL(advance + expectedLookupValue - 15, correctionIdleAdvance(advance));

    configPage2.idleAdvEnabled = IDLEADVANCE_MODE_SWITCHED;
    TEST_ASSERT_EQUAL(expectedLookupValue - 15, correctionIdleAdvance(advance));
}

static void test_correctionIdleAdvance_tps_lookup_nodelay(void) {
    setup_correctionIdleAdvance();

    setup_idleadv_tps();

    currentStatus.RPM = (currentStatus.CLIdleTarget * 10) + 100;
    assert_correctionIdleAdvance(8, 25);

    currentStatus.RPM = (currentStatus.CLIdleTarget * 10) - 100;
    assert_correctionIdleAdvance(-3, 15);
}

static void test_correctionIdleAdvance_ctps_lookup_nodelay(void) {
    setup_correctionIdleAdvance();

    setup_idleadv_ctps();

    currentStatus.RPM = (currentStatus.CLIdleTarget * 10) + 100;
    assert_correctionIdleAdvance(8, 25);

    currentStatus.RPM = (currentStatus.CLIdleTarget * 10) - 100;
    assert_correctionIdleAdvance(-3, 15);
}

static void test_correctionIdleAdvance_inactive_notrunning(void) {
    setup_correctionIdleAdvance();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    BIT_CLEAR(currentStatus.engine, BIT_ENGINE_RUN);
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_modeoff(void) {
    setup_correctionIdleAdvance();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    configPage2.idleAdvEnabled = IDLEADVANCE_MODE_OFF;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_rpmtoohigh(void) {
    setup_correctionIdleAdvance();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    currentStatus.RPM = (configPage2.idleAdvRPM * 100)+1;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_vsslimit(void) {
    setup_correctionIdleAdvance();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    configPage2.vssMode = 1;
    configPage2.idleAdvVss = 15;
    currentStatus.vss = configPage2.idleAdvVss + 1;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_tpslimit(void) {
    setup_correctionIdleAdvance();
    setup_idleadv_tps();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    currentStatus.TPS = configPage2.idleAdvTPS + 1U;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_ctpsinactive(void) {
    setup_correctionIdleAdvance();
    setup_idleadv_ctps();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    currentStatus.CTPSActive = 0;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_noadvance_rundelay(void) {
    setup_correctionIdleAdvance();
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
    runSecsX10 = (configPage2.idleAdvDelay * 5)-1;
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance_delay(void) {
    setup_correctionIdleAdvance();
    configPage9.idleAdvStartDelay = 3;
    BIT_SET(LOOP_TIMER, BIT_TIMER_10HZ);
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
    TEST_ASSERT_EQUAL(8, correctionIdleAdvance(8));
    TEST_ASSERT_EQUAL(23, correctionIdleAdvance(8));
}

static void test_correctionIdleAdvance(void) {
    RUN_TEST_P(test_correctionIdleAdvance_tps_lookup_nodelay);
    RUN_TEST_P(test_correctionIdleAdvance_ctps_lookup_nodelay);
    RUN_TEST_P(test_correctionIdleAdvance_inactive_notrunning);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_modeoff);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_rpmtoohigh);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_vsslimit);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_tpslimit);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_ctpsinactive);
    RUN_TEST_P(test_correctionIdleAdvance_noadvance_rundelay);
    RUN_TEST_P(test_correctionIdleAdvance_delay);
}

extern int8_t correctionSoftRevLimit(int8_t advance);

static void setup_correctionSoftRevLimit(void) {
    construct2dTables();
    initialiseCorrections();

    configPage6.engineProtectType = PROTECT_CUT_IGN;
    configPage4.SoftRevLim = 50;
    configPage4.SoftLimMax = 1;
    configPage4.SoftLimRetard = 5;
    configPage2.SoftLimitMode = SOFT_LIMIT_FIXED;

    currentStatus.RPMdiv100 = configPage4.SoftRevLim + 1;
    softLimitTime = 0;

    BIT_CLEAR(LOOP_TIMER, BIT_TIMER_10HZ);
}

static void assert_correctionSoftRevLimit(int8_t advance) {
    configPage2.SoftLimitMode = SOFT_LIMIT_FIXED;
    TEST_ASSERT_EQUAL(configPage4.SoftLimRetard, correctionSoftRevLimit(advance));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SFTLIM , currentStatus.status2);

    BIT_CLEAR(currentStatus.status2, BIT_STATUS2_SFTLIM);
    configPage2.SoftLimitMode = SOFT_LIMIT_RELATIVE;
    TEST_ASSERT_EQUAL(advance-configPage4.SoftLimRetard, correctionSoftRevLimit(advance));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SFTLIM , currentStatus.status2);
}

static void test_correctionSoftRevLimit_modes(void) {
    setup_correctionSoftRevLimit();

    assert_correctionSoftRevLimit(8);
    assert_correctionSoftRevLimit(-3);
}

static void test_correctionSoftRevLimit_inactive_protecttype(void) {
    setup_correctionSoftRevLimit();

    configPage6.engineProtectType = PROTECT_CUT_OFF;
    BIT_SET(currentStatus.status2, BIT_STATUS2_SFTLIM);
    TEST_ASSERT_EQUAL(8, correctionSoftRevLimit(8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SFTLIM , currentStatus.status2);

    configPage6.engineProtectType = PROTECT_CUT_FUEL;
    BIT_SET(currentStatus.status2, BIT_STATUS2_SFTLIM);
    TEST_ASSERT_EQUAL(8, correctionSoftRevLimit(8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SFTLIM , currentStatus.status2);
}

static void test_correctionSoftRevLimit_inactive_rpmtoohigh(void) {
    setup_correctionSoftRevLimit();
    assert_correctionSoftRevLimit(8);

    currentStatus.RPMdiv100 = configPage4.SoftRevLim-1;
    BIT_SET(currentStatus.status2, BIT_STATUS2_SFTLIM);
    TEST_ASSERT_EQUAL(8, correctionSoftRevLimit(8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SFTLIM , currentStatus.status2);
}

static void test_correctionSoftRevLimit_timeout(void) {
    setup_correctionSoftRevLimit();

    configPage4.SoftLimMax = 3;
    configPage2.SoftLimitMode = SOFT_LIMIT_RELATIVE;
    BIT_SET(LOOP_TIMER, BIT_TIMER_10HZ);
    TEST_ASSERT_EQUAL(8-configPage4.SoftLimRetard, correctionSoftRevLimit(8));
    TEST_ASSERT_EQUAL(-5-configPage4.SoftLimRetard, correctionSoftRevLimit(-5));
    TEST_ASSERT_EQUAL(23-configPage4.SoftLimRetard, correctionSoftRevLimit(23));
    TEST_ASSERT_EQUAL(-21, correctionSoftRevLimit(-21));
    TEST_ASSERT_EQUAL(8, correctionSoftRevLimit(8));
    TEST_ASSERT_EQUAL(0, correctionSoftRevLimit(0));
}

static void test_correctionSoftRevLimit(void) {
    RUN_TEST_P(test_correctionSoftRevLimit_modes);
    RUN_TEST_P(test_correctionSoftRevLimit_inactive_protecttype);
    RUN_TEST_P(test_correctionSoftRevLimit_inactive_rpmtoohigh);
    RUN_TEST_P(test_correctionSoftRevLimit_timeout);
}

extern int8_t correctionNitrous(int8_t advance);

static void test_correctionNitrous_disabled(void) {
    configPage10.n2o_enable = 0;
    TEST_ASSERT_EQUAL(13, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-13, correctionNitrous(-13));
}

static void test_correctionNitrous_stage1(void) {
    configPage10.n2o_enable = 1;
    configPage10.n2o_stage1_retard = 5;
    configPage10.n2o_stage2_retard = 0;
    
    currentStatus.nitrous_status = NITROUS_STAGE1;
    TEST_ASSERT_EQUAL(8, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-18, correctionNitrous(-13));
    
    currentStatus.nitrous_status = NITROUS_BOTH;
    TEST_ASSERT_EQUAL(8, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-18, correctionNitrous(-13));
}

static void test_correctionNitrous_stage2(void) {
    configPage10.n2o_enable = 1;
    configPage10.n2o_stage1_retard = 0;
    configPage10.n2o_stage2_retard = 5;
    
    currentStatus.nitrous_status = NITROUS_STAGE2;
    TEST_ASSERT_EQUAL(8, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-18, correctionNitrous(-13));
    
    currentStatus.nitrous_status = NITROUS_BOTH;
    TEST_ASSERT_EQUAL(8, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-18, correctionNitrous(-13));
}

static void test_correctionNitrous_stageboth(void) {
    configPage10.n2o_enable = 1;
    configPage10.n2o_stage1_retard = 3;
    configPage10.n2o_stage2_retard = 5;
      
    currentStatus.nitrous_status = NITROUS_BOTH;
    TEST_ASSERT_EQUAL(5, correctionNitrous(13));
    TEST_ASSERT_EQUAL(-21, correctionNitrous(-13));
}

static void test_correctionNitrous(void) {
    RUN_TEST_P(test_correctionNitrous_disabled);
    RUN_TEST_P(test_correctionNitrous_stage1);
    RUN_TEST_P(test_correctionNitrous_stage2);
    RUN_TEST_P(test_correctionNitrous_stageboth);
}

extern int8_t correctionSoftLaunch(int8_t advance);

static void setup_correctionSoftLaunch(void) {
    configPage6.launchEnabled = 1;
    configPage6.flatSArm = 20;
    configPage6.lnchSoftLim = 40;
    configPage10.lnchCtrlTPS = 80;
    configPage10.lnchCtrlVss = 50;
    configPage2.vssMode = 2;
    
    currentStatus.clutchTrigger = 1;
    currentStatus.clutchEngagedRPM = ((configPage6.flatSArm) * 100) - 100;
    currentStatus.RPM = ((configPage6.lnchSoftLim) * 100) + 100;
    currentStatus.TPS = configPage10.lnchCtrlTPS + 1;
    currentStatus.vss = 30;
}

static void test_correctionSoftLaunch_on(void) {
    setup_correctionSoftLaunch();

    configPage6.lnchRetard = -3;
    TEST_ASSERT_EQUAL(configPage6.lnchRetard, correctionSoftLaunch(-8));
    TEST_ASSERT_TRUE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SLAUNCH, currentStatus.status2);

    configPage6.lnchRetard = 3;
    currentStatus.launchingSoft = false;
    BIT_CLEAR(currentStatus.status2, BIT_STATUS2_SLAUNCH);
    TEST_ASSERT_EQUAL(configPage6.lnchRetard, correctionSoftLaunch(8));
    TEST_ASSERT_TRUE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_disabled(void) {
    setup_correctionSoftLaunch();
    configPage6.launchEnabled = 0;
    configPage6.lnchRetard = -3;

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_noclutchtrigger(void) {
    setup_correctionSoftLaunch();
    currentStatus.clutchTrigger = 0;
    configPage6.lnchRetard = -3;

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_clutchrpmlow(void) {
    setup_correctionSoftLaunch();
    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100) + 1;
    configPage6.lnchRetard = -3;

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_rpmlimit(void) {
    setup_correctionSoftLaunch();
    currentStatus.RPM = (configPage6.lnchSoftLim * 100) - 1;
    configPage6.lnchRetard = -3;

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_tpslow(void) {
    setup_correctionSoftLaunch();
    currentStatus.TPS = configPage10.lnchCtrlTPS - 1;
    configPage6.lnchRetard = -3;

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch_off_vsslimit(void) {
    setup_correctionSoftLaunch();
    currentStatus.vss = 100; //VSS above limit of 80

    TEST_ASSERT_EQUAL(-8, correctionSoftLaunch(-8));
    TEST_ASSERT_FALSE(currentStatus.launchingSoft);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_SLAUNCH, currentStatus.status2);
}

static void test_correctionSoftLaunch(void) {
    RUN_TEST_P(test_correctionSoftLaunch_on);
    RUN_TEST_P(test_correctionSoftLaunch_off_disabled);
    RUN_TEST_P(test_correctionSoftLaunch_off_noclutchtrigger);
    RUN_TEST_P(test_correctionSoftLaunch_off_clutchrpmlow);
    RUN_TEST_P(test_correctionSoftLaunch_off_rpmlimit);
    RUN_TEST_P(test_correctionSoftLaunch_off_tpslow);
    RUN_TEST_P(test_correctionSoftLaunch_off_vsslimit);
}

extern int8_t correctionSoftFlatShift(int8_t advance);

static void setup_correctionSoftFlatShift(void) {
    configPage6.flatSEnable = 1;
    configPage6.flatSArm = 10;
    configPage6.flatSSoftWin = 10;
    
    currentStatus.clutchTrigger = 1;
    currentStatus.clutchEngagedRPM = ((configPage6.flatSArm) * 100) + 500;
    currentStatus.RPM = currentStatus.clutchEngagedRPM + 600;

    BIT_CLEAR(currentStatus.status5, BIT_STATUS5_FLATSS);
}

static void test_correctionSoftFlatShift_on(void) {
    setup_correctionSoftFlatShift();
    configPage6.flatSRetard = -3;

    TEST_ASSERT_EQUAL(configPage6.flatSRetard, correctionSoftFlatShift(-8));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSS, currentStatus.status5);

    BIT_CLEAR(currentStatus.status5, BIT_STATUS5_FLATSS);
    TEST_ASSERT_EQUAL(configPage6.flatSRetard, correctionSoftFlatShift(3));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSS, currentStatus.status5);
}

static void test_correctionSoftFlatShift_off_disabled(void) {
    setup_correctionSoftFlatShift();
    configPage6.flatSRetard = -3;
    configPage6.flatSEnable = 0;

    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSS);
    TEST_ASSERT_EQUAL(-8, correctionSoftFlatShift(-8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSS, currentStatus.status5);
}

static void test_correctionSoftFlatShift_off_noclutchtrigger(void) {
    setup_correctionSoftFlatShift();
    configPage6.flatSRetard = -3;
    currentStatus.clutchTrigger = 0;

    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSS);
    TEST_ASSERT_EQUAL(-8, correctionSoftFlatShift(-8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSS, currentStatus.status5);
}

static void test_correctionSoftFlatShift_off_clutchrpmtoolow(void) {
    setup_correctionSoftFlatShift();
    configPage6.flatSRetard = -3;
    currentStatus.clutchEngagedRPM = ((configPage6.flatSArm) * 100) - 500;

    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSS);
    TEST_ASSERT_EQUAL(-8, correctionSoftFlatShift(-8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSS, currentStatus.status5);
}

static void test_correctionSoftFlatShift_off_rpmnotinwindow(void) {
    setup_correctionSoftFlatShift();
    configPage6.flatSRetard = -3;
    currentStatus.RPM = (currentStatus.clutchEngagedRPM - (configPage6.flatSSoftWin * 100) ) - 100;

    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSS);
    TEST_ASSERT_EQUAL(-8, correctionSoftFlatShift(-8));
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSS, currentStatus.status5);
}

static void test_correctionSoftFlatShift(void) {
    RUN_TEST_P(test_correctionSoftFlatShift_on);
    RUN_TEST_P(test_correctionSoftFlatShift_off_disabled);
    RUN_TEST_P(test_correctionSoftFlatShift_off_noclutchtrigger);
    RUN_TEST_P(test_correctionSoftFlatShift_off_clutchrpmtoolow);
    RUN_TEST_P(test_correctionSoftFlatShift_off_rpmnotinwindow);
}

static void setup_checkLaunchAndFlatShift(void) {
    initialiseCorrections();
    currentStatus.status2 = 0U;
    currentStatus.status5 = 0U;
    currentStatus.launchingHard = false;
    currentStatus.launchingSoft = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.clutchTrigger = true;
    currentStatus.previousClutchTrigger = true;
    currentStatus.clutchEngagedRPM = 0U;
    currentStatus.RPM = 0U;
    currentStatus.TPS = 0U;
    currentStatus.vss = 0U;

    configPage6.launchEnabled = 1;
    configPage6.flatSEnable = 1;
    configPage6.flatSArm = 20;
    configPage6.lnchHardLim = 40;
    configPage10.lnchCtrlTPS = 80;
    configPage10.lnchCtrlVss = 50;
    configPage2.vssMode = 0;
    configPage2.hardCutType = HARD_CUT_FULL;
    configPage15.rollingProtRPMDelta[0] = 0;

    pinLaunch = LED_BUILTIN;
    pinMode(pinLaunch, INPUT_PULLUP);
    configPage6.launchHiLo = 1;
}

static void test_checkLaunchAndFlatShift_sets_launch_hard(void) {
    setup_checkLaunchAndFlatShift();
    currentStatus.RPM = (configPage6.lnchHardLim * 100U) + 100U;
    currentStatus.TPS = configPage10.lnchCtrlTPS + 1U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_TRUE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HLAUNCH, currentStatus.status2);
    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSH, currentStatus.status5);
}

static void test_checkLaunchAndFlatShift_sets_flatshift_hard(void) {
    setup_checkLaunchAndFlatShift();
    currentStatus.clutchEngagedRPM = (configPage6.flatSArm * 100U) + 200U;
    currentStatus.RPM = currentStatus.clutchEngagedRPM + 100U;
    currentStatus.TPS = 0U;

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.launchingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_HLAUNCH, currentStatus.status2);
    TEST_ASSERT_TRUE(currentStatus.flatShiftingHard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSH, currentStatus.status5);
}

static void test_checkLaunchAndFlatShift_clears_flatshift_bit_when_inactive(void) {
    setup_checkLaunchAndFlatShift();
    currentStatus.flatShiftingHard = true;
    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSH);
    digitalWrite(pinLaunch, LOW);

    checkLaunchAndFlatShift();

    TEST_ASSERT_FALSE(currentStatus.flatShiftingHard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_FLATSH, currentStatus.status5);
}

static void test_checkLaunchAndFlatShift(void) {
    RUN_TEST_P(test_checkLaunchAndFlatShift_sets_launch_hard);
    RUN_TEST_P(test_checkLaunchAndFlatShift_sets_flatshift_hard);
    RUN_TEST_P(test_checkLaunchAndFlatShift_clears_flatshift_bit_when_inactive);
}

extern int8_t correctionKnockTiming(int8_t advance);

static void setup_correctionKnock(void) {
    construct2dTables();
    initialiseCorrections();

    configPage10.knock_mode = KNOCK_MODE_DIGITAL;
    configPage10.knock_count = 3U;
    configPage10.knock_firstStep = 3U;
    configPage10.knock_stepSize = 2U;
    configPage10.knock_maxRetard = 10U;
    configPage10.knock_stepTime = 1U;
    configPage10.knock_duration = 1U;
    configPage10.knock_recoveryStepTime = 1U;
    configPage10.knock_recoveryStep = 1U;
}

static void test_correctionKnock_disabled_modeoff(void) {
    setup_correctionKnock();
    configPage10.knock_mode = KNOCK_MODE_OFF;
    currentStatus.knockCount = configPage10.knock_count;

    TEST_ASSERT_EQUAL(-8, correctionKnockTiming(-8));
    TEST_ASSERT_EQUAL_UINT8(0, currentStatus.knockRetard);
}

static void test_correctionKnock_disabled_counttoolow(void) {
    setup_correctionKnock();
    currentStatus.knockCount = configPage10.knock_count - 1U;

    TEST_ASSERT_EQUAL(-8, correctionKnockTiming(-8));
    TEST_ASSERT_EQUAL_UINT8(0, currentStatus.knockRetard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_KNOCK_ACTIVE, currentStatus.status5);
}

static void test_correctionKnock_firstStep(void) {
    setup_correctionKnock();
    currentStatus.knockCount = configPage10.knock_count;

    TEST_ASSERT_EQUAL(-11, correctionKnockTiming(-8));
    TEST_ASSERT_EQUAL_UINT8(3U, currentStatus.knockRetard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_KNOCK_ACTIVE, currentStatus.status5);
}

static void test_correctionKnock_additionalStepAfterDelay(void) {
    setup_correctionKnock();
    currentStatus.knockCount = configPage10.knock_count;
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
    currentStatus.knockRetard = configPage10.knock_firstStep;
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE);
    knockState.startTime = micros() - ((configPage10.knock_stepTime * 1000UL) + 100UL);

    TEST_ASSERT_EQUAL(10 - 5, correctionKnockTiming(10));
    TEST_ASSERT_EQUAL_UINT8(configPage10.knock_count + 1U, currentStatus.knockCount);
    TEST_ASSERT_EQUAL_UINT8(5U, currentStatus.knockRetard);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_KNOCK_PULSE, currentStatus.status5);
}

static void test_correctionKnock_noAdditionalStepBeforeDelay(void) {
    setup_correctionKnock();
    currentStatus.knockCount = configPage10.knock_count;
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
    currentStatus.knockRetard = configPage10.knock_firstStep;
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE);
    knockState.startTime = micros();

    TEST_ASSERT_EQUAL(10 - configPage10.knock_firstStep, correctionKnockTiming(10));
    TEST_ASSERT_EQUAL_UINT8(configPage10.knock_count, currentStatus.knockCount);
    TEST_ASSERT_EQUAL_UINT8(configPage10.knock_firstStep, currentStatus.knockRetard);
}

static void test_correctionKnock_recoveryStep(void) {
    setup_correctionKnock();
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
    currentStatus.knockCount = configPage10.knock_count + 1U;
    currentStatus.knockRetard = 5U;
    knockState.lastRecoveryStep = 0U;
    knockState.startTime = micros() - ((configPage10.knock_duration * 100000UL) + (configPage10.knock_recoveryStepTime * 100000UL) + 100UL);

    TEST_ASSERT_EQUAL(10 - 4, correctionKnockTiming(10));
    TEST_ASSERT_EQUAL_UINT8(4U, currentStatus.knockRetard);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_KNOCK_ACTIVE, currentStatus.status5);
}

static void test_correctionKnock_recoveryCompletes(void) {
    setup_correctionKnock();
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
    currentStatus.knockCount = configPage10.knock_count + 1U;
    currentStatus.knockRetard = 2U;
    knockState.lastRecoveryStep = 0U;
    knockState.startTime = micros() - ((configPage10.knock_duration * 100000UL) + (3UL * configPage10.knock_recoveryStepTime * 100000UL) + 100UL);

    TEST_ASSERT_EQUAL(10, correctionKnockTiming(10));
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.knockRetard);
    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.knockCount);
    TEST_ASSERT_BIT_LOW(BIT_STATUS5_KNOCK_ACTIVE, currentStatus.status5);
}

static void test_correctionKnock(void) {
    RUN_TEST_P(test_correctionKnock_disabled_modeoff);
    RUN_TEST_P(test_correctionKnock_disabled_counttoolow);
    RUN_TEST_P(test_correctionKnock_firstStep);
    RUN_TEST_P(test_correctionKnock_additionalStepAfterDelay);
    RUN_TEST_P(test_correctionKnock_noAdditionalStepBeforeDelay);
    RUN_TEST_P(test_correctionKnock_recoveryStep);
    RUN_TEST_P(test_correctionKnock_recoveryCompletes);
}

static void setup_checkRevLimit(void) {
    construct2dTables();
    initialiseCorrections();
    currentStatus.status2 = 0U;
    currentStatus.engineProtectStatus = 0U;
    currentStatus.RPMdiv100 = 0U;
    softLimitTime = 0U;
    configPage6.engineProtectType = PROTECT_CUT_IGN;
}

static void test_checkRevLimit_off_returns_unlimited(void) {
    setup_checkRevLimit();
    configPage6.engineProtectType = PROTECT_CUT_OFF;
    currentStatus.RPMdiv100 = 200U;
    BIT_SET(currentStatus.status2, BIT_STATUS2_HRDLIM);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM);

    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, checkRevLimit());
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_HRDLIM, currentStatus.status2);
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_RPM, currentStatus.engineProtectStatus);
}

static void test_checkRevLimit_fixed_hard_limit_sets_flags(void) {
    setup_checkRevLimit();
    configPage9.hardRevMode = HARD_REV_FIXED;
    configPage4.HardRevLim = 60U;
    configPage4.SoftRevLim = 55U;
    configPage4.SoftLimMax = 3U;
    currentStatus.RPMdiv100 = 60U;

    TEST_ASSERT_EQUAL_UINT8(60U, checkRevLimit());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HRDLIM, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_RPM, currentStatus.engineProtectStatus);
}

static void test_checkRevLimit_fixed_soft_timeout_sets_flags(void) {
    setup_checkRevLimit();
    configPage9.hardRevMode = HARD_REV_FIXED;
    configPage4.HardRevLim = 70U;
    configPage4.SoftRevLim = 55U;
    configPage4.SoftLimMax = 2U;
    softLimitTime = 3U;
    currentStatus.RPMdiv100 = 55U;

    TEST_ASSERT_EQUAL_UINT8(70U, checkRevLimit());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HRDLIM, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_RPM, currentStatus.engineProtectStatus);
}

static void test_checkRevLimit_coolant_mode_sets_coolant_and_rpm_flags(void) {
    setup_checkRevLimit();
    configPage9.hardRevMode = HARD_REV_COOLANT;
    currentStatus.coolant = 85 - CALIBRATION_TEMPERATURE_OFFSET;
    currentStatus.RPMdiv100 = 54U;

    TEST_DATA_P uint8_t bins[] = { 60, 70, 80, 90, 100, 110 };
    TEST_DATA_P uint8_t values[] = { 65, 60, 55, 50, 45, 40 };
    populate_2dtable_P(&coolantProtectTable, values, bins);

    TEST_ASSERT_EQUAL_UINT8(53U, checkRevLimit());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HRDLIM, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_COOLANT, currentStatus.engineProtectStatus);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_RPM, currentStatus.engineProtectStatus);
}

static void test_checkRevLimit(void) {
    RUN_TEST_P(test_checkRevLimit_off_returns_unlimited);
    RUN_TEST_P(test_checkRevLimit_fixed_hard_limit_sets_flags);
    RUN_TEST_P(test_checkRevLimit_fixed_soft_timeout_sets_flags);
    RUN_TEST_P(test_checkRevLimit_coolant_mode_sets_coolant_and_rpm_flags);
}

static void setup_checkBoostLimit(void) {
    initialiseCorrections();
    currentStatus.status1 = 0U;
    currentStatus.status2 = 0U;
    currentStatus.engineProtectStatus = 0U;
    configPage6.boostCutEnabled = 1U;
    configPage6.boostLimit = 100U;
    currentStatus.MAP = (configPage6.boostLimit * 2U) + 10U;
}

static void test_checkBoostLimit_off_clears_flags(void) {
    setup_checkBoostLimit();
    configPage6.engineProtectType = PROTECT_CUT_OFF;
    BIT_SET(currentStatus.status1, BIT_STATUS1_BOOSTCUT);
    BIT_SET(currentStatus.status2, BIT_STATUS2_BOOSTCUT);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP);

    TEST_ASSERT_EQUAL_UINT8(0U, checkBoostLimit());
    TEST_ASSERT_BIT_LOW(BIT_STATUS1_BOOSTCUT, currentStatus.status1);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_BOOSTCUT, currentStatus.status2);
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_MAP, currentStatus.engineProtectStatus);
}

static void test_checkBoostLimit_ignition_only(void) {
    setup_checkBoostLimit();
    configPage6.engineProtectType = PROTECT_CUT_IGN;

    TEST_ASSERT_EQUAL_UINT8(1U, checkBoostLimit());
    TEST_ASSERT_BIT_LOW(BIT_STATUS1_BOOSTCUT, currentStatus.status1);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_BOOSTCUT, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_MAP, currentStatus.engineProtectStatus);
}

static void test_checkBoostLimit_fuel_only(void) {
    setup_checkBoostLimit();
    configPage6.engineProtectType = PROTECT_CUT_FUEL;

    TEST_ASSERT_EQUAL_UINT8(1U, checkBoostLimit());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS1_BOOSTCUT, currentStatus.status1);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_BOOSTCUT, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_MAP, currentStatus.engineProtectStatus);
}

static void test_checkBoostLimit_both(void) {
    setup_checkBoostLimit();
    configPage6.engineProtectType = PROTECT_CUT_BOTH;

    TEST_ASSERT_EQUAL_UINT8(1U, checkBoostLimit());
    TEST_ASSERT_BIT_HIGH(BIT_STATUS1_BOOSTCUT, currentStatus.status1);
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_BOOSTCUT, currentStatus.status2);
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_MAP, currentStatus.engineProtectStatus);
}

static void test_checkBoostLimit_below_threshold(void) {
    setup_checkBoostLimit();
    configPage6.engineProtectType = PROTECT_CUT_BOTH;
    currentStatus.MAP = (configPage6.boostLimit * 2U);

    TEST_ASSERT_EQUAL_UINT8(0U, checkBoostLimit());
    TEST_ASSERT_BIT_LOW(BIT_STATUS1_BOOSTCUT, currentStatus.status1);
    TEST_ASSERT_BIT_LOW(BIT_STATUS2_BOOSTCUT, currentStatus.status2);
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_MAP, currentStatus.engineProtectStatus);
}

static void test_checkBoostLimit(void) {
    RUN_TEST_P(test_checkBoostLimit_off_clears_flags);
    RUN_TEST_P(test_checkBoostLimit_ignition_only);
    RUN_TEST_P(test_checkBoostLimit_fuel_only);
    RUN_TEST_P(test_checkBoostLimit_both);
    RUN_TEST_P(test_checkBoostLimit_below_threshold);
}

static void setup_checkOilPressureLimit(void) {
    construct2dTables();
    initialiseCorrections();
    currentStatus.engineProtectStatus = 0U;
    configPage6.engineProtectType = PROTECT_CUT_OFF;
    currentStatus.oilPressure = 100U;
    checkOilPressureLimit();

    configPage6.engineProtectType = PROTECT_CUT_BOTH;
    configPage10.oilPressureProtEnbl = true;
    configPage10.oilPressureEnable = true;
    configPage10.oilPressureProtTime = 1U;
    currentStatus.RPMdiv100 = 50U;
    currentStatus.oilPressure = 10U;

    TEST_DATA_P uint8_t bins[] = { 20, 40, 60, 80, 100, 120 };
    TEST_DATA_P uint8_t values[] = { 20, 20, 20, 20, 20, 20 };
    populate_2dtable_P(&oilPressureProtectTable, values, bins);
}

static void test_checkOilPressureLimit_delays_and_clears(void) {
    setup_checkOilPressureLimit();

    TEST_ASSERT_EQUAL_UINT8(0U, checkOilPressureLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_OIL, currentStatus.engineProtectStatus);

    delay(50);
    TEST_ASSERT_EQUAL_UINT8(0U, checkOilPressureLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_OIL, currentStatus.engineProtectStatus);

    delay(70);
    TEST_ASSERT_EQUAL_UINT8(1U, checkOilPressureLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_OIL, currentStatus.engineProtectStatus);

    currentStatus.oilPressure = 30U;
    TEST_ASSERT_EQUAL_UINT8(0U, checkOilPressureLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_OIL, currentStatus.engineProtectStatus);
}

static void test_checkOilPressureLimit_handles_timer_wrap(void) {
    TEST_ASSERT_FALSE(protectionTimerElapsed(0U, 254U, 3U));
    TEST_ASSERT_TRUE(protectionTimerElapsed(1U, 254U, 3U));
}

static void test_checkOilPressureLimit(void) {
    RUN_TEST_P(test_checkOilPressureLimit_delays_and_clears);
    RUN_TEST_P(test_checkOilPressureLimit_handles_timer_wrap);
}

static void setup_checkAFRLimit(void) {
    initialiseCorrections();
    currentStatus.engineProtectStatus = 0U;
    configPage6.engineProtectType = PROTECT_CUT_OFF;
    configPage9.afrProtectEnabled = 0U;
    configPage6.egoType = EGO_TYPE_OFF;
    currentStatus.TPS = 0U;
    checkAFRLimit();

    configPage6.engineProtectType = PROTECT_CUT_BOTH;
    configPage9.afrProtectEnabled = 1U;
    configPage6.egoType = EGO_TYPE_WIDE;
    configPage9.afrProtectMinMAP = 100U;
    configPage9.afrProtectMinRPM = 30U;
    configPage9.afrProtectMinTPS = 50U;
    configPage9.afrProtectDeviation = 150U;
    configPage9.afrProtectCutTime = 1U;
    configPage9.afrProtectReactivationTPS = 20U;
    currentStatus.MAP = 220U;
    currentStatus.RPMdiv100 = 40U;
    currentStatus.TPS = 60U;
    currentStatus.O2 = 160U;
    currentStatus.afrTarget = 100U;
}

static void test_checkAFRLimit_delays_and_reactivates(void) {
    setup_checkAFRLimit();

    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    delay(50);
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    delay(70);
    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    currentStatus.TPS = configPage9.afrProtectReactivationTPS;
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);
}

static void test_checkAFRLimit_disabling_feature_clears_active_state(void) {
    setup_checkAFRLimit();
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    delay(120);

    TEST_ASSERT_EQUAL_UINT8(1U, checkAFRLimit());
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);

    configPage9.afrProtectEnabled = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, checkAFRLimit());
    TEST_ASSERT_BIT_LOW(ENGINE_PROTECT_BIT_AFR, currentStatus.engineProtectStatus);
}

static void test_checkAFRLimit(void) {
    RUN_TEST_P(test_checkAFRLimit_delays_and_reactivates);
    RUN_TEST_P(test_checkAFRLimit_disabling_feature_clears_active_state);
}

static void setup_logger_status_exports(void) {
    initialiseCorrections();
    currentStatus.status1 = 0U;
    currentStatus.status2 = 0U;
    currentStatus.engineProtectStatus = 0U;
    currentStatus.status5 = 0U;
    currentStatus.hasSync = false;
}

static void test_logger_exports_status1_bits(void) {
    setup_logger_status_exports();
    BIT_SET(currentStatus.status1, BIT_STATUS1_DFCO);
    BIT_SET(currentStatus.status1, BIT_STATUS1_BOOSTCUT);
    BIT_SET(currentStatus.status1, BIT_STATUS1_TOOTHLOG1READY);

    TEST_ASSERT_EQUAL_HEX8(currentStatus.status1, getTSLogEntry(1));
    TEST_ASSERT_EQUAL_INT16(currentStatus.status1, getReadableLogEntry(1));
    TEST_ASSERT_EQUAL_HEX8(currentStatus.status1, getLegacySecondarySerialLogEntry(1));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS1_DFCO, getTSLogEntry(1));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS1_BOOSTCUT, getReadableLogEntry(1));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS1_TOOTHLOG1READY, getLegacySecondarySerialLogEntry(1));
}

static void test_logger_exports_status2_bits(void) {
    setup_logger_status_exports();
    currentStatus.hasSync = true;
    BIT_SET(currentStatus.status2, BIT_STATUS2_HLAUNCH);
    BIT_SET(currentStatus.status2, BIT_STATUS2_SFTLIM);
    BIT_SET(currentStatus.status2, BIT_STATUS2_BOOSTCUT);
    BIT_SET(currentStatus.status2, BIT_STATUS2_SYNC);

    TEST_ASSERT_EQUAL_HEX8(currentStatus.status2, getTSLogEntry(32));
    TEST_ASSERT_EQUAL_INT16(currentStatus.status2, getReadableLogEntry(26));
    TEST_ASSERT_EQUAL_HEX8(currentStatus.status2, getLegacySecondarySerialLogEntry(31));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_HLAUNCH, getTSLogEntry(32));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SFTLIM, getReadableLogEntry(26));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_BOOSTCUT, getLegacySecondarySerialLogEntry(31));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS2_SYNC, getLegacySecondarySerialLogEntry(31));
}

static void test_logger_exports_engine_protection_bits(void) {
    setup_logger_status_exports();
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_RPM);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_MAP);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_OIL);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_AFR);
    BIT_SET(currentStatus.engineProtectStatus, ENGINE_PROTECT_BIT_COOLANT);

    TEST_ASSERT_EQUAL_HEX8(currentStatus.engineProtectStatus, getTSLogEntry(85));
    TEST_ASSERT_EQUAL_INT16(currentStatus.engineProtectStatus, getReadableLogEntry(58));
    TEST_ASSERT_EQUAL_HEX8(currentStatus.engineProtectStatus, getLegacySecondarySerialLogEntry(83));
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_RPM, getTSLogEntry(85));
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_MAP, getReadableLogEntry(58));
    TEST_ASSERT_BIT_HIGH(ENGINE_PROTECT_BIT_AFR, getLegacySecondarySerialLogEntry(83));
}

static void test_logger_exports_knock_status5_bits(void) {
    setup_logger_status_exports();
    BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSH);
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
    BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE);
    currentStatus.knockCount = 7U;
    currentStatus.knockRetard = 9U;

    TEST_ASSERT_EQUAL_HEX8(currentStatus.status5, getTSLogEntry(127));
    TEST_ASSERT_EQUAL_INT16(currentStatus.status5, getReadableLogEntry(91));
    TEST_ASSERT_EQUAL_HEX8(currentStatus.knockCount, getTSLogEntry(128));
    TEST_ASSERT_EQUAL_HEX8(currentStatus.knockRetard, getTSLogEntry(129));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_FLATSH, getTSLogEntry(127));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_KNOCK_ACTIVE, getTSLogEntry(127));
    TEST_ASSERT_BIT_HIGH(BIT_STATUS5_KNOCK_PULSE, getReadableLogEntry(91));
}

static void test_logger_status_exports(void) {
    RUN_TEST_P(test_logger_exports_status1_bits);
    RUN_TEST_P(test_logger_exports_status2_bits);
    RUN_TEST_P(test_logger_exports_engine_protection_bits);
    RUN_TEST_P(test_logger_exports_knock_status5_bits);
}

static void setup_legacy_packet_exports(void) {
    construct2dTables();
    initialiseCorrections();
    configPage6.iacAlgorithm = IAC_ALGORITHM_NONE;
    idleStepper.curIdleStep = 0;
    currentStatus.secl = 0x34U;
    currentStatus.PW1 = 0x2345U;
    currentStatus.PW2 = 0x3456U;
    currentStatus.RPM = 0x4567U;
    currentStatus.advance = 12;
    currentStatus.nSquirts = 4U;
    currentStatus.engine = 0x5AU;
    currentStatus.afrTarget = 147U;
    currentStatus.baro = 98U;
    currentStatus.MAP = 110U;
    currentStatus.IAT = 25U;
    currentStatus.coolant = 40U;
    currentStatus.TPS = 90U;
    currentStatus.battery10 = 142U;
    currentStatus.O2 = 145U;
    currentStatus.O2_2 = 150U;
    currentStatus.knockCount = 3U;
    currentStatus.knockRetard = 6U;
    currentStatus.egoCorrection = 101U;
    currentStatus.iatCorrection = 102U;
    currentStatus.wueCorrection = 103U;
    currentStatus.baroCorrection = 104U;
    currentStatus.corrections = 105U;
    currentStatus.VE = 80U;
    currentStatus.VE2 = 81U;
    currentStatus.tpsDOT = 0x0678U;
    currentStatus.mapDOT = 0x0789U;
    currentStatus.dwell = 35U;
    currentStatus.fuelLoad = 12U;
    currentStatus.advance1 = 7U;
    currentStatus.advance2 = 8U;
    currentStatus.outputsStatus = 0xA5U;

    TEST_DATA_P uint8_t bins[] = { 60, 70, 80, 90, 100, 110 };
    TEST_DATA_P uint8_t values[] = { 30, 25, 20, 15, 10, 5 };
    populate_2dtable_P(&CLTAdvanceTable, values, bins);
}

static void test_buildLegacyValuesPacket_maps_real_runtime_fields(void) {
    setup_legacy_packet_exports();
    uint8_t packet[114] = {0};

    TEST_ASSERT_EQUAL_UINT(114U, buildLegacyValuesPacket(packet, sizeof(packet)));
    TEST_ASSERT_EQUAL_HEX8(0x00, packet[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34, packet[1]);
    TEST_ASSERT_EQUAL_HEX8(currentStatus.knockCount, packet[32]);
    TEST_ASSERT_EQUAL_HEX8(currentStatus.knockRetard, packet[33]);
    TEST_ASSERT_EQUAL_HEX8(0x04, packet[46]);
    TEST_ASSERT_EQUAL_HEX8(0x10, packet[47]);
    TEST_ASSERT_EQUAL_HEX8(0x00, packet[70]);
    TEST_ASSERT_EQUAL_HEX8(0x46, packet[71]);
    TEST_ASSERT_EQUAL_HEX8(0x00, packet[72]);
    TEST_ASSERT_EQUAL_HEX8(0x50, packet[73]);
    TEST_ASSERT_EQUAL_HEX8(0x00, packet[56]);
    TEST_ASSERT_EQUAL_HEX8(0x32, packet[57]);
    TEST_ASSERT_EQUAL_HEX8(currentStatus.outputsStatus, packet[69]);
}

static void test_buildLegacyValuesPacket_preserves_placeholder_fields(void) {
    setup_legacy_packet_exports();
    uint8_t packet[114] = {0};

    buildLegacyValuesPacket(packet, sizeof(packet));
    TEST_ASSERT_EQUAL_HEX8(99U, packet[54]); // iacstep (when not using stepper)
    TEST_ASSERT_EQUAL_HEX8(99U, packet[55]); // iacstep (when not using stepper)
    TEST_ASSERT_EQUAL_HEX8(99U, packet[64]); // MAF (not supported in speed-density ECU)
    TEST_ASSERT_EQUAL_HEX8(99U, packet[65]); // MAF
}

static void test_buildLegacyValuesPacket_maps_flex_fuel_correction(void) {
    setup_legacy_packet_exports();
    currentStatus.flexCorrection = 115U; // 115% for E85

    uint8_t packet[114] = {0};
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(115U, packet[67]); // fuelcor (flex fuel correction %)
    TEST_ASSERT_EQUAL_HEX8(115U, packet[68]); // fuelcor
}

static void test_buildLegacyValuesPacket_maps_stepper_idle_position(void) {
    setup_legacy_packet_exports();
    configPage6.iacAlgorithm = IAC_ALGORITHM_STEP_CL;
    idleStepper.curIdleStep = 0x4567;

    uint8_t packet[114] = {0};
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(0x45, packet[54]);
    TEST_ASSERT_EQUAL_HEX8(0x67, packet[55]);
}

static void test_buildLegacyValuesPacket_maps_accel_enrich_and_fuel_cut(void) {
    setup_legacy_packet_exports();
    currentStatus.AEamount = 250U; // 250 / 2 = 125

    uint8_t packet[114] = {0};
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(125U, packet[42]); // accelEnrich byte 1
    TEST_ASSERT_EQUAL_HEX8(125U, packet[43]); // accelEnrich byte 2
    TEST_ASSERT_EQUAL_HEX8(100U, packet[44]); // tpsFuelCut byte 1 (100% = normal, no DFCO)
    TEST_ASSERT_EQUAL_HEX8(100U, packet[45]); // tpsFuelCut byte 2 (100% = normal, no DFCO)
}

static void test_buildLegacyValuesPacket_maps_wbo2_enable(void) {
    setup_legacy_packet_exports();
    configPage6.egoType = EGO_TYPE_WIDE;
    currentStatus.runSecs = 10;
    configPage6.ego_sdelay = 5;

    uint8_t packet[114] = {0};
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(1U, packet[14]); // wbo2_en1 (wideband enabled after warm-up)
    TEST_ASSERT_EQUAL_HEX8(1U, packet[15]); // wbo2_en2

    // Test before warm-up delay
    currentStatus.runSecs = 3;
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(0U, packet[14]); // wbo2_en1 (not warmed up)
    TEST_ASSERT_EQUAL_HEX8(0U, packet[15]); // wbo2_en2

    // Test with narrowband
    currentStatus.runSecs = 10;
    configPage6.egoType = EGO_TYPE_NARROW;
    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(0U, packet[14]); // wbo2_en1 (narrowband, not wideband)
    TEST_ASSERT_EQUAL_HEX8(0U, packet[15]); // wbo2_en2
}

static void test_buildLegacyValuesPacket_rejects_null_or_short_buffer(void) {
    uint8_t packet[113] = {0};

    TEST_ASSERT_EQUAL_UINT(0U, buildLegacyValuesPacket(nullptr, 114U));
    TEST_ASSERT_EQUAL_UINT(0U, buildLegacyValuesPacket(packet, sizeof(packet)));
}

static void test_buildLegacyValuesPacket_locks_target_and_pressure_offsets(void) {
    setup_legacy_packet_exports();
    uint8_t packet[114] = {0};

    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(currentStatus.afrTarget, packet[12]);
    TEST_ASSERT_EQUAL_HEX8(currentStatus.afrTarget, packet[13]);
    TEST_ASSERT_EQUAL_HEX8(0x03U, packet[16]); // baro * 10 = 980 = 0x03D4
    TEST_ASSERT_EQUAL_HEX8(0xD4U, packet[17]);
    TEST_ASSERT_EQUAL_HEX8(0x04U, packet[18]); // MAP * 10 = 1100 = 0x044C
    TEST_ASSERT_EQUAL_HEX8(0x4CU, packet[19]);
    TEST_ASSERT_EQUAL_HEX8(0x03U, packet[24]); // TPS * 10 = 900 = 0x0384
    TEST_ASSERT_EQUAL_HEX8(0x84U, packet[25]);
}

static void test_buildLegacyValuesPacket_locks_scaled_corrections_and_tail_fill(void) {
    setup_legacy_packet_exports();
    uint8_t packet[114];
    memset(packet, 0xAA, sizeof(packet));

    buildLegacyValuesPacket(packet, sizeof(packet));

    TEST_ASSERT_EQUAL_HEX8(0x03U, packet[34]); // egoCorrection * 10 = 1010 = 0x03F2
    TEST_ASSERT_EQUAL_HEX8(0xF2U, packet[35]);
    TEST_ASSERT_EQUAL_HEX8(0x03U, packet[38]); // iatCorrection * 10 = 1020 = 0x03FC
    TEST_ASSERT_EQUAL_HEX8(0xFCU, packet[39]);
    TEST_ASSERT_EQUAL_HEX8(0x04U, packet[40]); // wueCorrection * 10 = 1030 = 0x0406
    TEST_ASSERT_EQUAL_HEX8(0x06U, packet[41]);
    TEST_ASSERT_EQUAL_HEX8(120U, packet[66]); // fuelLoad * 10
    TEST_ASSERT_EQUAL_HEX8(99U, packet[74]);
    TEST_ASSERT_EQUAL_HEX8(99U, packet[113]);
}

static void test_parseLegacyPageReadRequest_valid(void) {
    const uint8_t buffer[] = { 0x00U, 0x04U, 0x34U, 0x12U, 0x78U, 0x56U };
    LegacyPageReadRequest request = {};

    TEST_ASSERT_TRUE(parseLegacyPageReadRequest(buffer, sizeof(buffer), &request));
    TEST_ASSERT_EQUAL_HEX8(0x04U, request.page);
    TEST_ASSERT_EQUAL_HEX16(0x1234U, request.offset);
    TEST_ASSERT_EQUAL_HEX16(0x5678U, request.length);
}

static void test_parseLegacyPageWriteRequest_valid(void) {
    const uint8_t buffer[] = { 0x00U, 0x09U, 0x78U, 0x56U, 0xBCU, 0x9AU };
    LegacyPageWriteRequest request = {};

    TEST_ASSERT_TRUE(parseLegacyPageWriteRequest(buffer, sizeof(buffer), &request));
    TEST_ASSERT_EQUAL_HEX8(0x09U, request.page);
    TEST_ASSERT_EQUAL_HEX16(0x5678U, request.offset);
    TEST_ASSERT_EQUAL_HEX16(0x9ABCU, request.length);
}

static void test_parseLegacyPageRequests_reject_short_or_null(void) {
    const uint8_t buffer[] = { 0x00U, 0x01U, 0x02U, 0x03U, 0x04U };
    LegacyPageReadRequest readRequest = {};
    LegacyPageWriteRequest writeRequest = {};

    TEST_ASSERT_FALSE(parseLegacyPageReadRequest(buffer, sizeof(buffer), &readRequest));
    TEST_ASSERT_FALSE(parseLegacyPageReadRequest(nullptr, 6U, &readRequest));
    TEST_ASSERT_FALSE(parseLegacyPageReadRequest(buffer, 6U, nullptr));
    TEST_ASSERT_FALSE(parseLegacyPageWriteRequest(buffer, sizeof(buffer), &writeRequest));
    TEST_ASSERT_FALSE(parseLegacyPageWriteRequest(nullptr, 6U, &writeRequest));
    TEST_ASSERT_FALSE(parseLegacyPageWriteRequest(buffer, 6U, nullptr));
}

static void test_getLegacyVersionResponseCommand_preserves_current_workarounds(void) {
    TEST_ASSERT_EQUAL('Q', getLegacyVersionResponseCommand('Q', false, SECONDARY_SERIAL_PROTO_GENERIC_FIXED));
    TEST_ASSERT_EQUAL('Q', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_MSDROID));
    TEST_ASSERT_EQUAL('Q', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_TUNERSTUDIO));
    TEST_ASSERT_EQUAL('S', getLegacyVersionResponseCommand('S', false, SECONDARY_SERIAL_PROTO_GENERIC_FIXED));
    TEST_ASSERT_EQUAL('Q', getLegacyVersionResponseCommand('S', true, SECONDARY_SERIAL_PROTO_MSDROID));
    TEST_ASSERT_EQUAL('S', getLegacyVersionResponseCommand('S', true, SECONDARY_SERIAL_PROTO_TUNERSTUDIO));
}

static void test_getLegacyVersionResponse_returns_expected_strings(void) {
    TEST_ASSERT_EQUAL(0, strcmp("speeduino 202501-T41", getLegacyVersionResponse('Q')));
    TEST_ASSERT_EQUAL(0, strcmp("Speeduino 2025.01.6", getLegacyVersionResponse('S')));
    TEST_ASSERT_NULL(getLegacyVersionResponse('X'));
}

static void test_parseLegacyCrcRequest_valid(void) {
    const uint8_t buffer[] = { 0x00U, 0x07U };
    LegacyCrcRequest request = {};

    TEST_ASSERT_TRUE(parseLegacyCrcRequest(buffer, sizeof(buffer), &request));
    TEST_ASSERT_EQUAL_HEX8(0x07U, request.page);
}

static void test_encodeLegacyCrc32Response_big_endian(void) {
    uint8_t buffer[4] = {0};

    encodeLegacyCrc32Response(0x12345678UL, buffer);
    TEST_ASSERT_EQUAL_HEX8(0x12U, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(0x34U, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(0x56U, buffer[2]);
    TEST_ASSERT_EQUAL_HEX8(0x78U, buffer[3]);
}

static void test_parseLegacyOutputChannelsRequest_valid(void) {
    const uint8_t buffer[] = { 0x5AU, 0x30U, 0x34U, 0x12U, 0x78U, 0x56U };
    LegacyOutputChannelsRequest request = {};

    TEST_ASSERT_TRUE(parseLegacyOutputChannelsRequest(buffer, sizeof(buffer), &request));
    TEST_ASSERT_EQUAL_HEX8(0x5AU, request.tsCanId);
    TEST_ASSERT_EQUAL_HEX8(0x30U, request.command);
    TEST_ASSERT_EQUAL_HEX16(0x1234U, request.offset);
    TEST_ASSERT_EQUAL_HEX16(0x5678U, request.length);
}

static void test_parseLegacyCrcAndOutputRequests_reject_short_or_null(void) {
    const uint8_t crcBuffer[] = { 0x00U };
    const uint8_t outputBuffer[] = { 0x00U, 0x30U, 0x01U, 0x02U, 0x03U };
    LegacyCrcRequest crcRequest = {};
    LegacyOutputChannelsRequest outputRequest = {};

    TEST_ASSERT_FALSE(parseLegacyCrcRequest(crcBuffer, sizeof(crcBuffer), &crcRequest));
    TEST_ASSERT_FALSE(parseLegacyCrcRequest(nullptr, 2U, &crcRequest));
    TEST_ASSERT_FALSE(parseLegacyCrcRequest(crcBuffer, 2U, nullptr));
    TEST_ASSERT_FALSE(parseLegacyOutputChannelsRequest(outputBuffer, sizeof(outputBuffer), &outputRequest));
    TEST_ASSERT_FALSE(parseLegacyOutputChannelsRequest(nullptr, 6U, &outputRequest));
    TEST_ASSERT_FALSE(parseLegacyOutputChannelsRequest(outputBuffer, 6U, nullptr));
}

static void test_isLegacyCommandByte_respects_protocol_gate(void) {
    TEST_ASSERT_TRUE(isLegacyCommandByte('F', false));
    TEST_ASSERT_TRUE(isLegacyCommandByte('A', true));
    TEST_ASSERT_TRUE(isLegacyCommandByte('?', true));
    TEST_ASSERT_FALSE(isLegacyCommandByte('A', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte('?', false));
    TEST_ASSERT_FALSE(isLegacyCommandByte(0x01U, true));
}

static void test_shouldRouteSecondaryTunerStudioToPrimary_requires_protocol_and_status_bit(void) {
    TEST_ASSERT_TRUE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, (1U << BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS)));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_TUNERSTUDIO, 0U));
    TEST_ASSERT_FALSE(shouldRouteSecondaryTunerStudioToPrimary(SECONDARY_SERIAL_PROTO_GENERIC_FIXED, (1U << BIT_STATUS5_ALLOW_TS_ON_SECONDARY_COMMS)));
}

static void test_getPrimaryLegacyHandlerDispatch_maps_shared_primary_commands(void) {
    const LegacyHandlerDispatch burnDispatch = getPrimaryLegacyHandlerDispatch('b', SECONDARY_SERIAL_PROTO_GENERIC_FIXED);
    const LegacyHandlerDispatch compatDispatch = getPrimaryLegacyHandlerDispatch('B', SECONDARY_SERIAL_PROTO_GENERIC_FIXED);
    const LegacyHandlerDispatch versionDispatch = getPrimaryLegacyHandlerDispatch('S', SECONDARY_SERIAL_PROTO_TUNERSTUDIO);
    const LegacyHandlerDispatch passthroughDispatch = getPrimaryLegacyHandlerDispatch('x', SECONDARY_SERIAL_PROTO_GENERIC_FIXED);

    TEST_ASSERT_TRUE(burnDispatch.handled);
    TEST_ASSERT_EQUAL_HEX8('b', burnDispatch.command);
    TEST_ASSERT_FALSE(burnDispatch.enableCompatMode);
    TEST_ASSERT_FALSE(burnDispatch.useSecondaryStatusFlag);

    TEST_ASSERT_TRUE(compatDispatch.handled);
    TEST_ASSERT_EQUAL_HEX8('B', compatDispatch.command);
    TEST_ASSERT_TRUE(compatDispatch.enableCompatMode);
    TEST_ASSERT_FALSE(compatDispatch.useSecondaryStatusFlag);

    TEST_ASSERT_TRUE(versionDispatch.handled);
    TEST_ASSERT_EQUAL_HEX8('Q', versionDispatch.command);
    TEST_ASSERT_FALSE(versionDispatch.enableCompatMode);
    TEST_ASSERT_TRUE(versionDispatch.useSecondaryStatusFlag);

    TEST_ASSERT_FALSE(passthroughDispatch.handled);
    TEST_ASSERT_EQUAL_HEX8('x', passthroughDispatch.command);
    TEST_ASSERT_FALSE(passthroughDispatch.enableCompatMode);
    TEST_ASSERT_FALSE(passthroughDispatch.useSecondaryStatusFlag);
}

static void test_decodeLegacyPageIdentifier_preserves_existing_ascii_and_binary_mapping(void) {
    TEST_ASSERT_EQUAL_UINT8(0U, decodeLegacyPageIdentifier('0'));
    TEST_ASSERT_EQUAL_UINT8(9U, decodeLegacyPageIdentifier('9'));
    TEST_ASSERT_EQUAL_UINT8(10U, decodeLegacyPageIdentifier('a'));
    TEST_ASSERT_EQUAL_UINT8(15U, decodeLegacyPageIdentifier('f'));
    TEST_ASSERT_EQUAL_UINT8(10U, decodeLegacyPageIdentifier('A'));
    TEST_ASSERT_EQUAL_UINT8(15U, decodeLegacyPageIdentifier('F'));
    TEST_ASSERT_EQUAL_UINT8(0x22U, decodeLegacyPageIdentifier(0x22U));
}

static void test_parseLegacyCommandButtonRequest_validates_and_builds_word(void) {
    static constexpr uint16_t testCommand = 769U;
    const uint8_t buffer[] = { highByte(testCommand), lowByte(testCommand) };
    LegacyCommandButtonRequest request = {};

    TEST_ASSERT_TRUE(parseLegacyCommandButtonRequest(buffer, sizeof(buffer), &request));
    TEST_ASSERT_EQUAL_UINT16(testCommand, request.command);
    TEST_ASSERT_FALSE(parseLegacyCommandButtonRequest(nullptr, sizeof(buffer), &request));
    TEST_ASSERT_FALSE(parseLegacyCommandButtonRequest(buffer, 1U, &request));
    TEST_ASSERT_FALSE(parseLegacyCommandButtonRequest(buffer, sizeof(buffer), nullptr));
}

static void test_legacyEepromTransferHeader_round_trips_and_validates(void) {
    uint8_t buffer[3] = {0};
    LegacyEepromTransferHeader header = {};

    TEST_ASSERT_EQUAL_UINT(3U, buildLegacyEepromTransferHeader(0x1234U, buffer, sizeof(buffer)));
    TEST_ASSERT_EQUAL_HEX8(0x34U, buffer[0]);
    TEST_ASSERT_EQUAL_HEX8(0x12U, buffer[1]);
    TEST_ASSERT_EQUAL_HEX8(',', buffer[2]);
    TEST_ASSERT_TRUE(parseLegacyEepromTransferHeader(buffer, sizeof(buffer), &header));
    TEST_ASSERT_EQUAL_HEX16(0x1234U, header.eepromSize);
    TEST_ASSERT_TRUE(isLegacyEepromTransferSizeValid(256U, 256U));
    TEST_ASSERT_FALSE(isLegacyEepromTransferSizeValid(255U, 256U));
}

static void test_legacyEepromTransferHeader_rejects_invalid_inputs(void) {
    uint8_t badHeader[] = { 0x34U, 0x12U, ';' };
    LegacyEepromTransferHeader header = {};

    TEST_ASSERT_FALSE(parseLegacyEepromTransferHeader(nullptr, sizeof(badHeader), &header));
    TEST_ASSERT_FALSE(parseLegacyEepromTransferHeader(badHeader, 2U, &header));
    TEST_ASSERT_FALSE(parseLegacyEepromTransferHeader(badHeader, sizeof(badHeader), nullptr));
    TEST_ASSERT_FALSE(parseLegacyEepromTransferHeader(badHeader, sizeof(badHeader), &header));
    TEST_ASSERT_EQUAL_UINT(0U, buildLegacyEepromTransferHeader(0x1234U, nullptr, 3U));
    TEST_ASSERT_EQUAL_UINT(0U, buildLegacyEepromTransferHeader(0x1234U, badHeader, 2U));
}

static void test_getLegacyLoggerCommand_maps_start_stop_and_ack(void) {
    const LegacyLoggerCommand startTooth = getLegacyLoggerCommand('H');
    const LegacyLoggerCommand stopTooth = getLegacyLoggerCommand('h');
    const LegacyLoggerCommand startComposite = getLegacyLoggerCommand('J');
    const LegacyLoggerCommand stopComposite = getLegacyLoggerCommand('j');
    const LegacyLoggerCommand startTertiary = getLegacyLoggerCommand('O');
    const LegacyLoggerCommand stopTertiary = getLegacyLoggerCommand('o');
    const LegacyLoggerCommand startCams = getLegacyLoggerCommand('X');
    const LegacyLoggerCommand stopCams = getLegacyLoggerCommand('x');
    const LegacyLoggerCommand unsupported = getLegacyLoggerCommand('Z');

    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_START_TOOTH, startTooth.action);
    TEST_ASSERT_TRUE(startTooth.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_STOP_TOOTH, stopTooth.action);
    TEST_ASSERT_FALSE(stopTooth.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_START_COMPOSITE, startComposite.action);
    TEST_ASSERT_TRUE(startComposite.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_STOP_COMPOSITE, stopComposite.action);
    TEST_ASSERT_FALSE(stopComposite.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_START_TERTIARY, startTertiary.action);
    TEST_ASSERT_TRUE(startTertiary.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_STOP_TERTIARY, stopTertiary.action);
    TEST_ASSERT_FALSE(stopTertiary.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_START_CAMS, startCams.action);
    TEST_ASSERT_TRUE(startCams.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_STOP_CAMS, stopCams.action);
    TEST_ASSERT_FALSE(stopCams.sendAck);
    TEST_ASSERT_EQUAL_UINT8(LEGACY_LOGGER_NONE, unsupported.action);
    TEST_ASSERT_FALSE(unsupported.sendAck);
}

static void test_getLegacyTriggeredLogType_prefers_tooth_then_composite(void) {
    TEST_ASSERT_EQUAL_UINT8(LEGACY_TRIGGERED_LOG_TOOTH, getLegacyTriggeredLogType(true, 2U));
    TEST_ASSERT_EQUAL_UINT8(LEGACY_TRIGGERED_LOG_COMPOSITE, getLegacyTriggeredLogType(false, 1U));
    TEST_ASSERT_EQUAL_UINT8(LEGACY_TRIGGERED_LOG_NONE, getLegacyTriggeredLogType(false, 0U));
}

static void test_isLegacyOutputChannelsCommandSupported_only_accepts_realtime_command(void) {
    TEST_ASSERT_TRUE(isLegacyOutputChannelsCommandSupported(LEGACY_SEND_OUTPUT_CHANNELS_CMD));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(LEGACY_FIXED_OUTPUT_CHANNELS_CMD));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(LEGACY_NEW_OUTPUT_CHANNELS_CMD));
    TEST_ASSERT_FALSE(isLegacyOutputChannelsCommandSupported(0x55U));
}

static void test_buildSecondaryRealtimeRequest_maps_commands_and_layout(void) {
    const SecondaryRealtimeRequest fixedRequest = buildSecondaryRealtimeRequest('A', SECONDARY_SERIAL_PROTO_GENERIC_FIXED);
    const SecondaryRealtimeRequest newRequest = buildSecondaryRealtimeRequest('n', SECONDARY_SERIAL_PROTO_GENERIC_INI);
    const SecondaryRealtimeRequest unsupportedRequest = buildSecondaryRealtimeRequest('x', SECONDARY_SERIAL_PROTO_GENERIC_FIXED);

    TEST_ASSERT_TRUE(fixedRequest.supported);
    TEST_ASSERT_EQUAL_UINT16(CAN_PACKET_SIZE, fixedRequest.packetLength);
    TEST_ASSERT_EQUAL_HEX8(LEGACY_FIXED_OUTPUT_CHANNELS_CMD, fixedRequest.cmd);
    TEST_ASSERT_TRUE(fixedRequest.useLegacyLayout);

    TEST_ASSERT_TRUE(newRequest.supported);
    TEST_ASSERT_EQUAL_UINT16(NEW_CAN_PACKET_SIZE, newRequest.packetLength);
    TEST_ASSERT_EQUAL_HEX8(LEGACY_NEW_OUTPUT_CHANNELS_CMD, newRequest.cmd);
    TEST_ASSERT_FALSE(newRequest.useLegacyLayout);

    TEST_ASSERT_FALSE(unsupportedRequest.supported);
    TEST_ASSERT_EQUAL_UINT16(0U, unsupportedRequest.packetLength);
    TEST_ASSERT_EQUAL_HEX8(0U, unsupportedRequest.cmd);
    TEST_ASSERT_FALSE(unsupportedRequest.useLegacyLayout);
}

static void test_buildSecondarySerialAck_matches_existing_commands(void) {
    SecondarySerialAck ack30 = buildSecondarySerialAck(SECONDARY_SERIAL_PROTO_GENERIC_FIXED, LEGACY_SEND_OUTPUT_CHANNELS_CMD);
    SecondarySerialAck ack31 = buildSecondarySerialAck(SECONDARY_SERIAL_PROTO_GENERIC_INI, LEGACY_FIXED_OUTPUT_CHANNELS_CMD);
    SecondarySerialAck ack32 = buildSecondarySerialAck(SECONDARY_SERIAL_PROTO_REALDASH, LEGACY_NEW_OUTPUT_CHANNELS_CMD);

    TEST_ASSERT_EQUAL_UINT(2U, ack30.length);
    TEST_ASSERT_EQUAL_HEX8('r', ack30.bytes[0]);
    TEST_ASSERT_EQUAL_HEX8(LEGACY_SEND_OUTPUT_CHANNELS_CMD, ack30.bytes[1]);

    TEST_ASSERT_EQUAL_UINT(1U, ack31.length);
    TEST_ASSERT_EQUAL_HEX8('A', ack31.bytes[0]);

    TEST_ASSERT_EQUAL_UINT(3U, ack32.length);
    TEST_ASSERT_EQUAL_HEX8('n', ack32.bytes[0]);
    TEST_ASSERT_EQUAL_HEX8(LEGACY_NEW_OUTPUT_CHANNELS_CMD, ack32.bytes[1]);
    TEST_ASSERT_EQUAL_HEX8(NEW_CAN_PACKET_SIZE, ack32.bytes[2]);
}

static void test_buildSecondarySerialAck_ignores_unsupported_protocols_and_commands(void) {
    SecondarySerialAck noProtoAck = buildSecondarySerialAck(SECONDARY_SERIAL_PROTO_MSDROID, LEGACY_SEND_OUTPUT_CHANNELS_CMD);
    SecondarySerialAck noCmdAck = buildSecondarySerialAck(SECONDARY_SERIAL_PROTO_GENERIC_FIXED, 0x55);

    TEST_ASSERT_EQUAL_UINT(0U, noProtoAck.length);
    TEST_ASSERT_EQUAL_UINT(0U, noCmdAck.length);
}

static void test_shouldThrottleLegacySerialWrites_only_for_primary_serial(void) {
    TEST_ASSERT_TRUE(shouldThrottleLegacySerialWrites(&Serial));
    TEST_ASSERT_FALSE(shouldThrottleLegacySerialWrites(&secondarySerial));
}

static void test_calculateLegacySendProgress_basic(void) {
    const LegacySendProgress progress = calculateLegacySendProgress(10U, 20U, 4U);

    TEST_ASSERT_EQUAL_UINT8(14U, progress.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(16U, progress.remainingLength);
}

static void test_calculateLegacySendProgress_clamps_to_byte_limits(void) {
    const LegacySendProgress progress = calculateLegacySendProgress(250U, 255U, 20U);

    TEST_ASSERT_EQUAL_UINT8(UINT8_MAX, progress.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(235U, progress.remainingLength);
}

static void test_calculateLegacySendProgress_zero_when_complete(void) {
    const LegacySendProgress progress = calculateLegacySendProgress(50U, 10U, 10U);

    TEST_ASSERT_EQUAL_UINT8(60U, progress.nextOffset);
    TEST_ASSERT_EQUAL_UINT8(0U, progress.remainingLength);
}

static void test_buildLegacySerialResumeRequest_matches_resume_command_layout(void) {
    const LegacySerialResumeRequest request = buildLegacySerialResumeRequest(12U, 34U);

    TEST_ASSERT_EQUAL_HEX8(LEGACY_SEND_OUTPUT_CHANNELS_CMD, request.cmd);
    TEST_ASSERT_EQUAL_UINT8(12U, request.offset);
    TEST_ASSERT_EQUAL_UINT8(34U, request.length);
}

static void test_legacy_packet_exports(void) {
    RUN_TEST_P(test_buildLegacyValuesPacket_maps_real_runtime_fields);
    RUN_TEST_P(test_buildLegacyValuesPacket_preserves_placeholder_fields);
    RUN_TEST_P(test_buildLegacyValuesPacket_maps_flex_fuel_correction);
    RUN_TEST_P(test_buildLegacyValuesPacket_maps_stepper_idle_position);
    RUN_TEST_P(test_buildLegacyValuesPacket_maps_accel_enrich_and_fuel_cut);
    RUN_TEST_P(test_buildLegacyValuesPacket_maps_wbo2_enable);
    RUN_TEST_P(test_buildLegacyValuesPacket_rejects_null_or_short_buffer);
    RUN_TEST_P(test_buildLegacyValuesPacket_locks_target_and_pressure_offsets);
    RUN_TEST_P(test_buildLegacyValuesPacket_locks_scaled_corrections_and_tail_fill);
    RUN_TEST_P(test_parseLegacyPageReadRequest_valid);
    RUN_TEST_P(test_parseLegacyPageWriteRequest_valid);
    RUN_TEST_P(test_parseLegacyPageRequests_reject_short_or_null);
    RUN_TEST_P(test_getLegacyVersionResponseCommand_preserves_current_workarounds);
    RUN_TEST_P(test_getLegacyVersionResponse_returns_expected_strings);
    RUN_TEST_P(test_parseLegacyCrcRequest_valid);
    RUN_TEST_P(test_encodeLegacyCrc32Response_big_endian);
    RUN_TEST_P(test_parseLegacyOutputChannelsRequest_valid);
    RUN_TEST_P(test_parseLegacyCrcAndOutputRequests_reject_short_or_null);
    RUN_TEST_P(test_isLegacyCommandByte_respects_protocol_gate);
    RUN_TEST_P(test_shouldRouteSecondaryTunerStudioToPrimary_requires_protocol_and_status_bit);
    RUN_TEST_P(test_getPrimaryLegacyHandlerDispatch_maps_shared_primary_commands);
    RUN_TEST_P(test_decodeLegacyPageIdentifier_preserves_existing_ascii_and_binary_mapping);
    RUN_TEST_P(test_parseLegacyCommandButtonRequest_validates_and_builds_word);
    RUN_TEST_P(test_legacyEepromTransferHeader_round_trips_and_validates);
    RUN_TEST_P(test_legacyEepromTransferHeader_rejects_invalid_inputs);
    RUN_TEST_P(test_getLegacyLoggerCommand_maps_start_stop_and_ack);
    RUN_TEST_P(test_getLegacyTriggeredLogType_prefers_tooth_then_composite);
    RUN_TEST_P(test_isLegacyOutputChannelsCommandSupported_only_accepts_realtime_command);
    RUN_TEST_P(test_buildSecondaryRealtimeRequest_maps_commands_and_layout);
    RUN_TEST_P(test_buildSecondarySerialAck_matches_existing_commands);
    RUN_TEST_P(test_buildSecondarySerialAck_ignores_unsupported_protocols_and_commands);
    RUN_TEST_P(test_shouldThrottleLegacySerialWrites_only_for_primary_serial);
    RUN_TEST_P(test_calculateLegacySendProgress_basic);
    RUN_TEST_P(test_calculateLegacySendProgress_clamps_to_byte_limits);
    RUN_TEST_P(test_calculateLegacySendProgress_zero_when_complete);
    RUN_TEST_P(test_buildLegacySerialResumeRequest_matches_resume_command_layout);
}

static void setup_correctionsDwell(void) {
    construct2dTables();
    initialiseCorrections();
    
    configPage4.sparkDur = 10;
    configPage2.perToothIgn = false;
    configPage4.dwellErrCorrect = 0;
    configPage4.sparkMode = IGN_MODE_WASTED;

    currentStatus.actualDwell = 770;
    currentStatus.battery10 = 95;

    revolutionTime = 666;

    TEST_DATA_P uint8_t bins[] = { 60,  70,  80,  90,  100, 110 };
    TEST_DATA_P uint8_t values[] = { 130, 125, 120, 115, 110, 90 };
    populate_2dtable_P(&dwellVCorrectionTable, values, bins);   
}

static void test_correctionsDwell_nopertooth(void) {
    setup_correctionsDwell();

    currentStatus.battery10 = 105;
    configPage2.nCylinders = 8;

    configPage4.sparkMode = IGN_MODE_WASTED;
    TEST_ASSERT_EQUAL(296, correctionsDwell(800));

    configPage4.sparkMode = IGN_MODE_SINGLE;
    TEST_ASSERT_EQUAL(74, correctionsDwell(800));

    configPage4.sparkMode = IGN_MODE_ROTARY;
    configPage10.rotaryType = ROTARY_IGN_RX8;
    TEST_ASSERT_EQUAL(296, correctionsDwell(800));

    configPage4.sparkMode = IGN_MODE_ROTARY;
    configPage10.rotaryType = ROTARY_IGN_FC;
    TEST_ASSERT_EQUAL(74, correctionsDwell(800));
}

static void test_correctionsDwell_pertooth(void) {
    setup_correctionsDwell();

    currentStatus.battery10 = 105;
    configPage2.perToothIgn = true;
    configPage4.dwellErrCorrect = 1;
    configPage4.sparkMode = IGN_MODE_WASTED;

    currentStatus.actualDwell = 200;
    TEST_ASSERT_EQUAL(444, correctionsDwell(800));

    currentStatus.actualDwell = 1400;
    TEST_ASSERT_EQUAL(296, correctionsDwell(800));
}

static void test_correctionsDwell_wasted_nopertooth_largerevolutiontime(void) {
    setup_correctionsDwell();

    currentStatus.dwellCorrection = 55;
    currentStatus.battery10 = 105;
    revolutionTime = 5000;
    TEST_ASSERT_EQUAL(800, correctionsDwell(800));
}

static void test_correctionsDwell_initialises_current_actualDwell(void) {
    setup_correctionsDwell();

    currentStatus.actualDwell = 0;
    correctionsDwell(777);
    TEST_ASSERT_EQUAL(777, currentStatus.actualDwell);
}

static void test_correctionsDwell_sets_dwellCorrection(void) {
    setup_correctionsDwell();

    currentStatus.dwellCorrection = UINT8_MAX;
    currentStatus.battery10 = 90;
    correctionsDwell(777);
    TEST_ASSERT_EQUAL(115, currentStatus.dwellCorrection);
}

static void test_correctionsDwell_uses_batvcorrection(void) {
    setup_correctionsDwell();
    configPage2.nCylinders = 8;
    configPage4.sparkMode = IGN_MODE_WASTED;

    currentStatus.battery10 = 105;
    TEST_ASSERT_EQUAL(296, correctionsDwell(800));

    currentStatus.battery10 = 65;
    TEST_ASSERT_EQUAL(337, correctionsDwell(800));
}

static void test_correctionsDwell(void) {
    RUN_TEST_P(test_correctionsDwell_nopertooth);
    RUN_TEST_P(test_correctionsDwell_pertooth);
    RUN_TEST_P(test_correctionsDwell_wasted_nopertooth_largerevolutiontime);
    RUN_TEST_P(test_correctionsDwell_initialises_current_actualDwell);
    RUN_TEST_P(test_correctionsDwell_sets_dwellCorrection);
    RUN_TEST_P(test_correctionsDwell_uses_batvcorrection);
}

void testIgnCorrections(void) {
    Unity.TestFile = __FILE__;

    test_correctionFixedTiming();
    test_correctionCLTadvance();
    test_correctionCrankingFixedTiming();
    test_correctionFlexTiming();
    test_correctionWMITiming();
    test_correctionIATretard();
    test_correctionIdleAdvance();
    test_correctionSoftRevLimit();
    test_correctionNitrous();
    test_correctionSoftLaunch();
    test_correctionSoftFlatShift();
    test_checkLaunchAndFlatShift();
    test_correctionKnock();
    test_checkRevLimit();
    test_checkBoostLimit();
    test_checkOilPressureLimit();
    test_checkAFRLimit();
    test_logger_status_exports();
    test_legacy_packet_exports();
    // correctionDFCOignition() is tested in the fueling unit tests, since it is tightly coupled to fuel DFCO
    test_correctionsDwell();
}
