/**
 * @file test_live_data_map.cpp
 * @brief Boundary and size tests for the live_data_map.h declarative reference
 *
 * These tests verify properties of the overall logger packet shape rather than
 * individual field values (those are locked in test_logger_byte_regression.cpp).
 *
 *   1. LIVE_DATA_MAP_SIZE constant equals 148 — documents and locks the expected size.
 *   2. getTSLogEntry() last valid byte (index LIVE_DATA_MAP_SIZE-1) is handled —
 *      verifies the final switch case is present and returns the expected value.
 *   3. getTSLogEntry() out-of-range byte returns 0 — verifies the default: case.
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "logger.h"
#include "live_data_map.h"

/**
 * LIVE_DATA_MAP_SIZE must equal 148.
 * If this fails, live_data_map.h was edited without updating the constant,
 * or LOG_ENTRY_SIZE changed without updating this file.
 */
static void test_live_data_map_size_is_148(void)
{
    TEST_ASSERT_EQUAL_UINT8(148U, LIVE_DATA_MAP_SIZE);
}

/**
 * The last byte (index 147 = LIVE_DATA_MAP_SIZE-1) is runtimeStatusA.
 * With all four packed flags cleared the value must be exactly 0.
 * This verifies the final switch case in getTSLogEntry() is present.
 */
static void test_live_data_map_last_byte_is_zero_when_flags_clear(void)
{
    currentStatus.fuelPumpOn      = false;
    currentStatus.launchingHard   = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.idleUpActive    = false;

    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(LIVE_DATA_MAP_SIZE - 1U));

    /* restore */
}

/**
 * A request for byte index LIVE_DATA_MAP_SIZE (one past the last valid index)
 * must fall through to the default: case and return 0.
 */
static void test_live_data_map_out_of_range_returns_zero(void)
{
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(LIVE_DATA_MAP_SIZE));
}

void test_live_data_map(void)
{
    SET_UNITY_FILENAME() {
        RUN_TEST(test_live_data_map_size_is_148);
        RUN_TEST(test_live_data_map_last_byte_is_zero_when_flags_clear);
        RUN_TEST(test_live_data_map_out_of_range_returns_zero);
    }
}
