#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "idle.h"
#include "table2d.h"

// These globals are defined in idle.cpp but not declared in idle.h
extern struct table2D iacStepTable;
extern struct table2D iacCrankStepsTable;
extern byte idleInitComplete;
extern uint8_t idleTaper;

static const uint8_t CRANK_STEPS = 20U; // stored in table; code multiplies by 3 → targetIdleStep = 60
static const uint8_t RUN_STEPS   = 10U; // stored in table; code multiplies by 3 → targetIdleStep = 30
// With coolant=0, lookup key = 0 + CALIBRATION_TEMPERATURE_OFFSET = 40
static const uint8_t TEST_BIN    = (uint8_t)(0U + CALIBRATION_TEMPERATURE_OFFSET);

static void setup_ol_step_tables(void)
{
    currentStatus.engine       = 0U;
    currentStatus.coolant      = 0;
    currentStatus.idleLoad     = 0U;
    currentStatus.RPM          = 0U;
    currentStatus.airConStatus = 0U;

    configPage6.iacAlgorithm  = IAC_ALGORITHM_STEP_OL;
    configPage6.iacStepHome   = 0U;   // isStepperHomed() returns true immediately (0*3 == 0 < 0 is false for unsigned)
    configPage6.iacStepHyster = 0U;   // doStep() steps unconditionally when error != 0
    configPage6.iacPWMrun     = false;

    configPage9.iacMaxSteps    = 200U; // 200*3=600, safely above all test values
    configPage9.iacStepperInv  = 0U;
    configPage9.iacStepperPower = 0U;  // STEPPER_POWER_WHEN_ACTIVE=0; power cut when inactive

    configPage2.idleUpEnabled  = false;
    configPage2.idleTaperTime  = 10U;

    configPage15.airConIdleSteps = 0U;

    idleTaper  = 10U; // taper complete by default
    LOOP_TIMER = 0U;  // all timer bits clear by default

    idleStepper.stepperStatus  = SOFF;
    idleStepper.curIdleStep    = 0;
    idleStepper.targetIdleStep = 0;
    idleStepper.stepStartTime  = 0UL;

    // Replicate the table pointer setup from initialiseIdle() for STEP_OL,
    // then fill with predictable uniform values.
    iacCrankStepsTable.xSize     = 4U;
    iacCrankStepsTable.valueSize = SIZE_BYTE;
    iacCrankStepsTable.axisSize  = SIZE_BYTE;
    iacCrankStepsTable.values    = configPage6.iacCrankSteps;
    iacCrankStepsTable.axisX     = configPage6.iacCrankBins;
    populate_2dtable(&iacCrankStepsTable, CRANK_STEPS, TEST_BIN);

    iacStepTable.xSize     = 10U;
    iacStepTable.valueSize = SIZE_BYTE;
    iacStepTable.axisSize  = SIZE_BYTE;
    iacStepTable.values    = configPage6.iacOLStepVal;
    iacStepTable.axisX     = configPage6.iacBins;
    populate_2dtable(&iacStepTable, RUN_STEPS, TEST_BIN);

    // Skip initialiseIdle() inside idleControl() — tables are already set above.
    idleInitComplete = IAC_ALGORITHM_STEP_OL;
}

// Not running (engine=0): targetIdleStep from cranking table (* 3)
static void test_idle_step_ol_not_running_uses_crank_table(void)
{
    setup_ol_step_tables();
    // engine = 0: !BIT_ENGINE_RUN is true

    idleControl();

    TEST_ASSERT_EQUAL_INT(CRANK_STEPS * 3, idleStepper.targetIdleStep);
}

// Cranking (BIT_ENGINE_CRANK set, BIT_ENGINE_RUN clear): also uses cranking table
static void test_idle_step_ol_cranking_uses_crank_table(void)
{
    setup_ol_step_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_CRANK);
    currentStatus.RPM = 400U;

    idleControl();

    TEST_ASSERT_EQUAL_INT(CRANK_STEPS * 3, idleStepper.targetIdleStep);
}

// Running, taper complete: targetIdleStep from running table (* 3)
static void test_idle_step_ol_running_taper_done_uses_run_table(void)
{
    setup_ol_step_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    currentStatus.RPM = 1000U;
    idleTaper = configPage2.idleTaperTime; // taper == taperTime → not active
    LOOP_TIMER |= (1U << BIT_TIMER_10HZ);  // 10 Hz tick required for running path

    idleControl();

    TEST_ASSERT_EQUAL_INT(RUN_STEPS * 3, idleStepper.targetIdleStep);
}

// Running, taper at start (idleTaper=0): map(0, 0, taperTime, crankVal*3, runVal*3) == crankVal*3
static void test_idle_step_ol_running_taper_start_maps_to_crank(void)
{
    setup_ol_step_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    currentStatus.RPM = 1000U;
    idleTaper = 0U;
    configPage2.idleTaperTime = 10U;
    LOOP_TIMER |= (1U << BIT_TIMER_10HZ);

    idleControl();

    // map(0, 0, 10, CRANK_STEPS*3, RUN_STEPS*3) == CRANK_STEPS*3
    TEST_ASSERT_EQUAL_INT(CRANK_STEPS * 3, idleStepper.targetIdleStep);
}

// Max-steps clamp: targetIdleStep saturates at iacMaxSteps * 3 even when run table exceeds it
static void test_idle_step_ol_max_steps_clamped(void)
{
    setup_ol_step_tables();
    BIT_SET(currentStatus.engine, BIT_ENGINE_RUN);
    currentStatus.RPM = 1000U;
    idleTaper = configPage2.idleTaperTime; // taper done → run table (RUN_STEPS*3 = 30)
    LOOP_TIMER |= (1U << BIT_TIMER_10HZ);
    configPage9.iacMaxSteps = 1U; // 1*3=3, well below RUN_STEPS*3=30

    idleControl();

    TEST_ASSERT_EQUAL_INT(1 * 3, idleStepper.targetIdleStep);
}

void testIdleOLSteps(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST_P(test_idle_step_ol_not_running_uses_crank_table);
        RUN_TEST_P(test_idle_step_ol_cranking_uses_crank_table);
        RUN_TEST_P(test_idle_step_ol_running_taper_done_uses_run_table);
        RUN_TEST_P(test_idle_step_ol_running_taper_start_maps_to_crank);
        RUN_TEST_P(test_idle_step_ol_max_steps_clamped);
    }
}
