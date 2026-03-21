#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "sensors.h"
#include "decoders.h"

extern void construct2dTables(void);
extern bool knockAngleIsInsideWindow(int currentAngle, int windowStart, int windowDuration, int angleMax);
extern bool knockWindowIsActive(const statuses &current, const config10 &page10, int currentAngle);

static int fakeCrankAngle = 0;

static int fakeGetCrankAngle(void)
{
  return fakeCrankAngle;
}

static void setup_knock_window(void)
{
  construct2dTables();

  TEST_DATA_P uint8_t bins[] = { 10, 20, 30, 40, 50, 60 };
  TEST_DATA_P uint8_t startAngles[] = { 20, 20, 20, 20, 20, 20 };
  TEST_DATA_P uint8_t durations[] = { 15, 15, 15, 15, 15, 15 };
  populate_2dtable_P(&knockWindowStartTable, startAngles, bins);
  populate_2dtable_P(&knockWindowDurationTable, durations, bins);

  currentStatus.hasSync = true;
  currentStatus.status3 = 0U;
  currentStatus.RPMdiv100 = 25;
  currentStatus.MAP = 100;

  configPage10.knock_maxMAP = 80;
  configPage10.knock_maxRPM = 90;
  configPage10.knock_mode = KNOCK_MODE_DIGITAL;

  getCrankAngle = fakeGetCrankAngle;
  fakeCrankAngle = 0;
}

static void test_knockAngleIsInsideWindow_basic(void)
{
  TEST_ASSERT_TRUE(knockAngleIsInsideWindow(25, 20, 15, 360));
  TEST_ASSERT_TRUE(knockAngleIsInsideWindow(34, 20, 15, 360));
  TEST_ASSERT_FALSE(knockAngleIsInsideWindow(35, 20, 15, 360));
  TEST_ASSERT_FALSE(knockAngleIsInsideWindow(19, 20, 15, 360));
}

static void test_knockAngleIsInsideWindow_wrap(void)
{
  TEST_ASSERT_TRUE(knockAngleIsInsideWindow(355, 350, 20, 360));
  TEST_ASSERT_TRUE(knockAngleIsInsideWindow(5, 350, 20, 360));
  TEST_ASSERT_FALSE(knockAngleIsInsideWindow(25, 350, 20, 360));
}

static void test_knockWindowIsActive(void)
{
  setup_knock_window();

  TEST_ASSERT_TRUE(knockWindowIsActive(currentStatus, configPage10, 25));
  TEST_ASSERT_FALSE(knockWindowIsActive(currentStatus, configPage10, 45));

  currentStatus.hasSync = false;
  TEST_ASSERT_FALSE(knockWindowIsActive(currentStatus, configPage10, 25));
}

static void test_knockEventIsValid(void)
{
  setup_knock_window();

  fakeCrankAngle = 25;
  TEST_ASSERT_TRUE(knockEventIsValid());

  fakeCrankAngle = 45;
  TEST_ASSERT_FALSE(knockEventIsValid());

  fakeCrankAngle = 25;
  currentStatus.MAP = (configPage10.knock_maxMAP * 2L) + 1L;
  TEST_ASSERT_FALSE(knockEventIsValid());
}

static void test_knockEventIsValid_suppressedDuringLaunchOrFlatShift(void)
{
  setup_knock_window();
  fakeCrankAngle = 25;

  configPage10.knock_limiterDisable = 1U;
  currentStatus.launchingSoft = true;
  TEST_ASSERT_FALSE(knockEventIsValid());

  currentStatus.launchingSoft = false;
  currentStatus.flatShiftingHard = true;
  TEST_ASSERT_FALSE(knockEventIsValid());

  currentStatus.flatShiftingHard = false;
  BIT_SET(currentStatus.status2, BIT_STATUS2_HLAUNCH);
  TEST_ASSERT_FALSE(knockEventIsValid());

  BIT_CLEAR(currentStatus.status2, BIT_STATUS2_HLAUNCH);
  BIT_SET(currentStatus.status5, BIT_STATUS5_FLATSS);
  TEST_ASSERT_FALSE(knockEventIsValid());
}

static void test_knockEventIsValid_notSuppressedWhenOptionDisabled(void)
{
  setup_knock_window();
  fakeCrankAngle = 25;

  configPage10.knock_limiterDisable = 0U;
  currentStatus.launchingSoft = true;
  TEST_ASSERT_TRUE(knockEventIsValid());
}

static void test_knockPulse_obeys_window(void)
{
  setup_knock_window();
  currentStatus.knockCount = 0;
  currentStatus.status5 = 0U;

  fakeCrankAngle = 45;
  knockPulse();
  TEST_ASSERT_EQUAL_UINT8(0, currentStatus.knockCount);
  TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE));

  fakeCrankAngle = 25;
  knockPulse();
  TEST_ASSERT_EQUAL_UINT8(1, currentStatus.knockCount);
  TEST_ASSERT_TRUE(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE));
}

static void test_knockPulse_suppressedDuringLaunch(void)
{
  setup_knock_window();
  currentStatus.knockCount = 0;
  currentStatus.status5 = 0U;
  configPage10.knock_limiterDisable = 1U;
  currentStatus.launchingHard = true;
  fakeCrankAngle = 25;

  knockPulse();

  TEST_ASSERT_EQUAL_UINT8(0, currentStatus.knockCount);
  TEST_ASSERT_FALSE(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE));
}

void test_knock_window(void)
{
  SET_UNITY_FILENAME() {
    RUN_TEST(test_knockAngleIsInsideWindow_basic);
    RUN_TEST(test_knockAngleIsInsideWindow_wrap);
    RUN_TEST(test_knockWindowIsActive);
    RUN_TEST(test_knockEventIsValid);
    RUN_TEST(test_knockEventIsValid_suppressedDuringLaunchOrFlatShift);
    RUN_TEST(test_knockEventIsValid_notSuppressedWhenOptionDisabled);
    RUN_TEST(test_knockPulse_obeys_window);
    RUN_TEST(test_knockPulse_suppressedDuringLaunch);
  }
}
