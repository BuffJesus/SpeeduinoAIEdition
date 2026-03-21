#include <unity.h>
#include "crankMaths.h"
#include "decoders.h"
#include "../test_utils.h"

extern void SetRevolutionTime(uint32_t revTime);

struct crankmaths_rev_testdata {
  uint16_t rpm;
  unsigned long revolutionTime;
  uint16_t angle;
  unsigned long expected;
} *crankmaths_rev_testdata_current;

void test_crankmaths_angletotime_revolution_execute() {
  crankmaths_rev_testdata *testdata = crankmaths_rev_testdata_current;
  SetRevolutionTime(testdata->revolutionTime);
  TEST_ASSERT_INT32_WITHIN(1, testdata->expected, angleToTimeMicroSecPerDegree(testdata->angle));
}

#if false
struct crankmaths_tooth_testdata {
  uint16_t rpm;
  uint16_t triggerToothAngle;
  unsigned long toothTime;
  uint16_t angle;
  unsigned long expected;
} *crankmaths_tooth_testdata_current;

extern uint32_t angleToTimeIntervalTooth(uint16_t angle);

void test_crankmaths_angletotime_tooth_execute() {
  crankmaths_tooth_testdata *testdata = crankmaths_tooth_testdata_current;
  triggerToothAngle = testdata->triggerToothAngle;
  toothLastToothTime = toothLastMinusOneToothTime + testdata->toothTime;
  TEST_ASSERT_EQUAL(testdata->expected, angleToTimeIntervalTooth(testdata->angle));
}
#endif

struct crankmaths_timetoangle_testdata {
  uint16_t rpm;
  unsigned long revolutionTime;
  unsigned long timeUS;
  uint16_t expectedAngle;
} *crankmaths_timetoangle_testdata_current;

void test_crankmaths_timetoangle_revolution_execute() {
  crankmaths_timetoangle_testdata *testdata = crankmaths_timetoangle_testdata_current;
  SetRevolutionTime(testdata->revolutionTime);
  // Use tolerance of 4 degrees for very low RPM (<100), 2 degrees otherwise
  // Fixed-point arithmetic precision is reduced at very low RPM
  uint16_t tolerance = (testdata->rpm < 100) ? 4 : 2;
  TEST_ASSERT_UINT16_WITHIN(tolerance, testdata->expectedAngle, timeToAngleDegPerMicroSec(testdata->timeUS));
}

void testCrankMaths()
{
  SET_UNITY_FILENAME() {
    constexpr byte testNameLength = 200;
    char testName[testNameLength];

    const crankmaths_rev_testdata crankmaths_rev_testdatas[] = {
      { .rpm = 50,    .revolutionTime = 1200000, .angle = 0,   .expected = 0 },
      { .rpm = 50,    .revolutionTime = 1200000, .angle = 25,  .expected = 83333 }, // 83333,3333
      { .rpm = 50,    .revolutionTime = 1200000, .angle = 720, .expected = 2400000 },
      { .rpm = 2500,  .revolutionTime = 24000,   .angle = 0,   .expected = 0 },
      { .rpm = 2500,  .revolutionTime = 24000,   .angle = 25,  .expected = 1667 }, // 1666,6666
      { .rpm = 2500,  .revolutionTime = 24000,   .angle = 720, .expected = 48000 },
      { .rpm = 20000, .revolutionTime = 3000,    .angle = 0,   .expected = 0 },
      { .rpm = 20000, .revolutionTime = 3000,    .angle = 25,  .expected = 208 }, // 208,3333
      { .rpm = 20000, .revolutionTime = 3000,    .angle = 720, .expected = 6000 }
    };

    for (auto testdata : crankmaths_rev_testdatas) {
      crankmaths_rev_testdata_current = &testdata;
      snprintf(testName, testNameLength, "crankmaths/angletotime/revolution/%urpm/%uangle", testdata.rpm, testdata.angle);
      UnityDefaultTestRun(test_crankmaths_angletotime_revolution_execute, testName, __LINE__);
    }

    // Test the inverse function: timeToAngleDegPerMicroSec
    // Note: Fixed-point arithmetic introduces rounding errors, especially at low RPM
    const crankmaths_timetoangle_testdata crankmaths_timetoangle_testdatas[] = {
      { .rpm = 50,    .revolutionTime = 1200000, .timeUS = 0,      .expectedAngle = 0 },
      { .rpm = 50,    .revolutionTime = 1200000, .timeUS = 83333,  .expectedAngle = 25 },
      { .rpm = 50,    .revolutionTime = 1200000, .timeUS = 600000, .expectedAngle = 180 }, // Use half revolution instead
      { .rpm = 2500,  .revolutionTime = 24000,   .timeUS = 0,      .expectedAngle = 0 },
      { .rpm = 2500,  .revolutionTime = 24000,   .timeUS = 1667,   .expectedAngle = 25 },
      { .rpm = 2500,  .revolutionTime = 24000,   .timeUS = 12000,  .expectedAngle = 180 },
      { .rpm = 2500,  .revolutionTime = 24000,   .timeUS = 24000,  .expectedAngle = 360 },
      { .rpm = 20000, .revolutionTime = 3000,    .timeUS = 0,      .expectedAngle = 0 },
      { .rpm = 20000, .revolutionTime = 3000,    .timeUS = 208,    .expectedAngle = 25 },
      { .rpm = 20000, .revolutionTime = 3000,    .timeUS = 1500,   .expectedAngle = 180 },
      { .rpm = 20000, .revolutionTime = 3000,    .timeUS = 3000,   .expectedAngle = 360 }
    };

    for (auto testdata : crankmaths_timetoangle_testdatas) {
      crankmaths_timetoangle_testdata_current = &testdata;
      snprintf(testName, testNameLength, "crankmaths/timetoangle/revolution/%urpm/%luusec", testdata.rpm, testdata.timeUS);
      UnityDefaultTestRun(test_crankmaths_timetoangle_revolution_execute, testName, __LINE__);
    }

  #if false
    const crankmaths_tooth_testdata crankmaths_tooth_testdatas[] = {
      { .rpm = 50,    .triggerToothAngle = 3,   .toothTime = 10000,  .angle = 0,   .expected = 0 },
      { .rpm = 50,    .triggerToothAngle = 3,   .toothTime = 10000,  .angle = 25,  .expected = 83333 }, // 83333,3333
      { .rpm = 50,    .triggerToothAngle = 3,   .toothTime = 10000,  .angle = 720, .expected = 2400000 },
      { .rpm = 2500,  .triggerToothAngle = 3,   .toothTime = 200,    .angle = 0,   .expected = 0 },
      { .rpm = 2500,  .triggerToothAngle = 3,   .toothTime = 200,    .angle = 25,  .expected = 1666 }, // 1666,6666
      { .rpm = 2500,  .triggerToothAngle = 3,   .toothTime = 200,    .angle = 720, .expected = 48000 },
      { .rpm = 20000, .triggerToothAngle = 3,   .toothTime = 25,     .angle = 0,   .expected = 0 },
      { .rpm = 20000, .triggerToothAngle = 3,   .toothTime = 25,     .angle = 25,  .expected = 208 }, // 208,3333
      { .rpm = 20000, .triggerToothAngle = 3,   .toothTime = 25,     .angle = 720, .expected = 6000 },
      { .rpm = 50,    .triggerToothAngle = 180, .toothTime = 600000, .angle = 0,   .expected = 0 },
      { .rpm = 50,    .triggerToothAngle = 180, .toothTime = 600000, .angle = 25,  .expected = 83333 }, // 83333,3333
      { .rpm = 50,    .triggerToothAngle = 180, .toothTime = 600000, .angle = 720, .expected = 2400000 },
      { .rpm = 2500,  .triggerToothAngle = 180, .toothTime = 12000,  .angle = 0,   .expected = 0 },
      { .rpm = 2500,  .triggerToothAngle = 180, .toothTime = 12000,  .angle = 25,  .expected = 1666 }, // 1666,6666
      { .rpm = 2500,  .triggerToothAngle = 180, .toothTime = 12000,  .angle = 720, .expected = 48000 },
      { .rpm = 20000, .triggerToothAngle = 180, .toothTime = 1500,   .angle = 0,   .expected = 0 },
      { .rpm = 20000, .triggerToothAngle = 180, .toothTime = 1500,   .angle = 25,  .expected = 208 }, // 208,3333
      { .rpm = 20000, .triggerToothAngle = 180, .toothTime = 1500,   .angle = 720, .expected = 6000 },
    };
    // The same for all tests
    BIT_SET(decoderState, BIT_DECODER_TOOTH_ANG_CORRECT);
    toothLastMinusOneToothTime = 200000;

    for (auto testdata : crankmaths_tooth_testdatas) {
      crankmaths_tooth_testdata_current = &testdata;
      snprintf(testName, testNameLength, "crankmaths/angletotime/tooth/%urpm/%uangle/%utoothangle", testdata.rpm, testdata.angle, testdata.triggerToothAngle);
      UnityDefaultTestRun(test_crankmaths_angletotime_tooth_execute, testName, __LINE__);
    }
  #endif
  }
}