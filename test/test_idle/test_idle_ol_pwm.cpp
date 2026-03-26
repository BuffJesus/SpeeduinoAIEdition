#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "idle.h"
#include "table2d.h"

// These globals are defined in idle.cpp but not declared in idle.h
extern struct table2D iacPWMTable;
extern struct table2D iacCrankDutyTable;
extern byte idleInitComplete;
extern uint8_t idleTaper;

static const uint8_t CRANK_LOAD = 60U;
static const uint8_t RUN_LOAD   = 30U;
// With coolant=0, lookup key = 0 + CALIBRATION_TEMPERATURE_OFFSET = 40
static const uint8_t TEST_BIN   = (uint8_t)(0U + CALIBRATION_TEMPERATURE_OFFSET);

static void setup_ol_pwm_tables(void)
{
    currentStatus.engine    = 0U;
    currentStatus.coolant   = 0;
    currentStatus.idleLoad  = 0U;
    currentStatus.RPM       = 0U;
    currentStatus.airConStatus = 0U;

    configPage6.iacAlgorithm = IAC_ALGORITHM_PWM_OL;
    configPage6.iacPWMrun    = false;
    configPage15.airConIdleSteps = 0U;

    configPage2.idleUpEnabled = false;
    configPage2.idleTaperTime = 10U;

    idleTaper = 10U; // taper complete by default

    idle_pwm_max_count = 100U;

    // Replicate the table pointer setup from initialiseIdle() for PWM_OL,
    // then fill with predictable uniform values.
    iacCrankDutyTable.xSize     = 4U;
    iacCrankDutyTable.valueSize = SIZE_BYTE;
    iacCrankDutyTable.axisSize  = SIZE_BYTE;
    iacCrankDutyTable.values    = configPage6.iacCrankDuty;
    iacCrankDutyTable.axisX     = configPage6.iacCrankBins;
    populate_2dtable(&iacCrankDutyTable, CRANK_LOAD, TEST_BIN);

    iacPWMTable.xSize     = 10U;
    iacPWMTable.valueSize = SIZE_BYTE;
    iacPWMTable.axisSize  = SIZE_BYTE;
    iacPWMTable.values    = configPage6.iacOLPWMVal;
    iacPWMTable.axisX     = configPage6.iacBins;
    populate_2dtable(&iacPWMTable, RUN_LOAD, TEST_BIN);

    // Skip initialiseIdle() inside idleControl() — tables are already set above.
    idleInitComplete = IAC_ALGORITHM_PWM_OL;
}

// Cranking: idleLoad taken from cranking table
static void test_idle_ol_pwm_cranking_uses_crank_table(void)
{
    setup_ol_pwm_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_CRANK);

    idleControl();

    TEST_ASSERT_EQUAL_UINT8(CRANK_LOAD, currentStatus.idleLoad);
}

// Not running + iacPWMrun=true: idleLoad taken from cranking table
static void test_idle_ol_pwm_not_running_pwmrun_uses_crank_table(void)
{
    setup_ol_pwm_tables();
    configPage6.iacPWMrun = true;
    // engine=0: neither cranking nor running

    idleControl();

    TEST_ASSERT_EQUAL_UINT8(CRANK_LOAD, currentStatus.idleLoad);
}

// Not running + iacPWMrun=false: idleLoad stays at 0
static void test_idle_ol_pwm_not_running_no_pwmrun_zero_load(void)
{
    setup_ol_pwm_tables();
    configPage6.iacPWMrun = false;
    // engine=0: neither cranking nor running, no pwmRun fallback

    idleControl();

    TEST_ASSERT_EQUAL_UINT8(0U, currentStatus.idleLoad);
}

// Running, taper complete: idleLoad from running table
static void test_idle_ol_pwm_running_taper_done_uses_run_table(void)
{
    setup_ol_pwm_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    idleTaper = configPage2.idleTaperTime; // taper == taperTime → not active

    idleControl();

    TEST_ASSERT_EQUAL_UINT8(RUN_LOAD, currentStatus.idleLoad);
}

// Running, taper at start (idleTaper=0): map(0,0,taperTime,crankVal,runVal) = crankVal
static void test_idle_ol_pwm_running_taper_start_maps_to_crank_value(void)
{
    setup_ol_pwm_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    idleTaper = 0U;
    configPage2.idleTaperTime = 10U;
    // map(0, 0, 10, CRANK_LOAD, RUN_LOAD) == CRANK_LOAD

    idleControl();

    TEST_ASSERT_EQUAL_UINT8(CRANK_LOAD, currentStatus.idleLoad);
}

void testIdleOLPWM(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_idle_ol_pwm_cranking_uses_crank_table);
        RUN_TEST_P(test_idle_ol_pwm_not_running_pwmrun_uses_crank_table);
        RUN_TEST_P(test_idle_ol_pwm_not_running_no_pwmrun_zero_load);
        RUN_TEST_P(test_idle_ol_pwm_running_taper_done_uses_run_table);
        RUN_TEST_P(test_idle_ol_pwm_running_taper_start_maps_to_crank_value);
    }
}
