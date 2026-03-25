/**
 * @file test_logger_byte_regression.cpp
 * @brief Regression tests locking firmware logger byte positions to TunerStudio bit mappings
 *
 * These tests prevent silent drift between logger.cpp byte assignments and INI ochBlockSize
 * channel mappings for high-risk status channels. Any change to byte position of status3,
 * engineProtectStatus, status5, knockCount, or knockRetard in getTSLogEntry() or
 * getReadableLogEntry() will cause a test failure here.
 *
 * getTSLogEntry() byte locks:
 *   Byte 84  -> currentStatus.status3       (halfsync, fuel2, nitrous, etc.)
 *   Byte 85  -> currentStatus.engineProtectStatus (RPM/MAP/oil/AFR/coolant bits)
 *   Byte 127 -> currentStatus.status5       (KNOCK_ACTIVE, KNOCK_PULSE, flat-shift)
 *   Byte 128 -> currentStatus.knockCount
 *   Byte 129 -> currentStatus.knockRetard
 *
 * getReadableLogEntry() case index locks:
 *   Index 57 -> currentStatus.status3
 *   Index 58 -> currentStatus.engineProtectStatus
 *   Index 91 -> currentStatus.status5
 *   Index 92 -> currentStatus.knockCount
 *   Index 93 -> currentStatus.knockRetard
 */

#include <unity.h>
#include "../test_utils.h"
#include "globals.h"
#include "logger.h"

// ========= getTSLogEntry byte position locks =========

static void test_logger_byte_84_exports_status3(void)
{
    currentStatus.status3 = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(84));

    currentStatus.status3 = (1U << BIT_STATUS3_HALFSYNC);
    TEST_ASSERT_EQUAL_UINT8((1U << BIT_STATUS3_HALFSYNC), getTSLogEntry(84));

    currentStatus.status3 = 0xA5U;
    TEST_ASSERT_EQUAL_UINT8(0xA5U, getTSLogEntry(84));

    currentStatus.status3 = 0U; // restore
}

static void test_logger_byte_85_exports_engine_protect_status(void)
{
    currentStatus.engineProtectStatus = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(85));

    currentStatus.engineProtectStatus = (1U << ENGINE_PROTECT_BIT_RPM);
    TEST_ASSERT_EQUAL_UINT8((1U << ENGINE_PROTECT_BIT_RPM), getTSLogEntry(85));

    currentStatus.engineProtectStatus = (1U << ENGINE_PROTECT_BIT_MAP) | (1U << ENGINE_PROTECT_BIT_COOLANT);
    TEST_ASSERT_EQUAL_UINT8((1U << ENGINE_PROTECT_BIT_MAP) | (1U << ENGINE_PROTECT_BIT_COOLANT), getTSLogEntry(85));

    currentStatus.engineProtectStatus = 0U; // restore
}

static void test_logger_byte_127_exports_status5(void)
{
    currentStatus.status5 = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(127));

    currentStatus.status5 = (1U << BIT_STATUS5_KNOCK_ACTIVE);
    TEST_ASSERT_EQUAL_UINT8((1U << BIT_STATUS5_KNOCK_ACTIVE), getTSLogEntry(127));

    currentStatus.status5 = (1U << BIT_STATUS5_KNOCK_PULSE);
    TEST_ASSERT_EQUAL_UINT8((1U << BIT_STATUS5_KNOCK_PULSE), getTSLogEntry(127));

    currentStatus.status5 = (1U << BIT_STATUS5_KNOCK_ACTIVE) | (1U << BIT_STATUS5_KNOCK_PULSE);
    TEST_ASSERT_EQUAL_UINT8((1U << BIT_STATUS5_KNOCK_ACTIVE) | (1U << BIT_STATUS5_KNOCK_PULSE), getTSLogEntry(127));

    currentStatus.status5 = 0U; // restore
}

static void test_logger_byte_128_exports_knock_count(void)
{
    currentStatus.knockCount = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(128));

    currentStatus.knockCount = 7U;
    TEST_ASSERT_EQUAL_UINT8(7U, getTSLogEntry(128));

    currentStatus.knockCount = 255U;
    TEST_ASSERT_EQUAL_UINT8(255U, getTSLogEntry(128));

    currentStatus.knockCount = 0U; // restore
}

static void test_logger_byte_129_exports_knock_retard(void)
{
    currentStatus.knockRetard = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(129));

    currentStatus.knockRetard = 10U;
    TEST_ASSERT_EQUAL_UINT8(10U, getTSLogEntry(129));

    currentStatus.knockRetard = 255U;
    TEST_ASSERT_EQUAL_UINT8(255U, getTSLogEntry(129));

    currentStatus.knockRetard = 0U; // restore
}

// ========= getReadableLogEntry case index locks =========

static void test_readable_index_57_exports_status3(void)
{
    currentStatus.status3 = 0U;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(57));

    currentStatus.status3 = (1U << BIT_STATUS3_HALFSYNC);
    TEST_ASSERT_EQUAL_INT16((1U << BIT_STATUS3_HALFSYNC), getReadableLogEntry(57));

    currentStatus.status3 = 0xFFU;
    TEST_ASSERT_EQUAL_INT16(0xFF, getReadableLogEntry(57));

    currentStatus.status3 = 0U; // restore
}

static void test_readable_index_58_exports_engine_protect_status(void)
{
    currentStatus.engineProtectStatus = 0U;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(58));

    currentStatus.engineProtectStatus = (1U << ENGINE_PROTECT_BIT_RPM) | (1U << ENGINE_PROTECT_BIT_MAP);
    TEST_ASSERT_EQUAL_INT16((1U << ENGINE_PROTECT_BIT_RPM) | (1U << ENGINE_PROTECT_BIT_MAP), getReadableLogEntry(58));

    currentStatus.engineProtectStatus = 0U; // restore
}

static void test_readable_index_91_exports_status5(void)
{
    currentStatus.status5 = 0U;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(91));

    currentStatus.status5 = (1U << BIT_STATUS5_KNOCK_ACTIVE) | (1U << BIT_STATUS5_KNOCK_PULSE);
    TEST_ASSERT_EQUAL_INT16((1U << BIT_STATUS5_KNOCK_ACTIVE) | (1U << BIT_STATUS5_KNOCK_PULSE), getReadableLogEntry(91));

    currentStatus.status5 = 0U; // restore
}

static void test_readable_index_92_exports_knock_count(void)
{
    currentStatus.knockCount = 0U;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(92));

    currentStatus.knockCount = 3U;
    TEST_ASSERT_EQUAL_INT16(3, getReadableLogEntry(92));

    currentStatus.knockCount = 0U; // restore
}

static void test_readable_index_93_exports_knock_retard(void)
{
    currentStatus.knockRetard = 0U;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(93));

    currentStatus.knockRetard = 15U;
    TEST_ASSERT_EQUAL_INT16(15, getReadableLogEntry(93));

    currentStatus.knockRetard = 0U; // restore
}

void test_logger_byte_regression(void)
{
    SET_UNITY_FILENAME() {
        // getTSLogEntry byte position locks
        RUN_TEST(test_logger_byte_84_exports_status3);
        RUN_TEST(test_logger_byte_85_exports_engine_protect_status);
        RUN_TEST(test_logger_byte_127_exports_status5);
        RUN_TEST(test_logger_byte_128_exports_knock_count);
        RUN_TEST(test_logger_byte_129_exports_knock_retard);
        // getReadableLogEntry case index locks
        RUN_TEST(test_readable_index_57_exports_status3);
        RUN_TEST(test_readable_index_58_exports_engine_protect_status);
        RUN_TEST(test_readable_index_91_exports_status5);
        RUN_TEST(test_readable_index_92_exports_knock_count);
        RUN_TEST(test_readable_index_93_exports_knock_retard);
    }
}
