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

// ========= Phase 8E: PW5-PW8, launchCorrection, injAngle byte/index locks =========

static void test_logger_bytes_132_139_export_pw5_pw8(void)
{
    // PW5: bytes 132 (low) and 133 (high)
    currentStatus.PW5 = 0x1234U;
    TEST_ASSERT_EQUAL_UINT8(0x34U, getTSLogEntry(132));
    TEST_ASSERT_EQUAL_UINT8(0x12U, getTSLogEntry(133));
    // PW6: bytes 134-135
    currentStatus.PW6 = 0xABCDU;
    TEST_ASSERT_EQUAL_UINT8(0xCDU, getTSLogEntry(134));
    TEST_ASSERT_EQUAL_UINT8(0xABU, getTSLogEntry(135));
    // PW7: bytes 136-137
    currentStatus.PW7 = 500U;
    TEST_ASSERT_EQUAL_UINT8(lowByte(500U),  getTSLogEntry(136));
    TEST_ASSERT_EQUAL_UINT8(highByte(500U), getTSLogEntry(137));
    // PW8: bytes 138-139
    currentStatus.PW8 = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(138));
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(139));
    // restore
    currentStatus.PW5 = 0U; currentStatus.PW6 = 0U;
    currentStatus.PW7 = 0U; currentStatus.PW8 = 0U;
}

static void test_logger_byte_140_exports_launch_correction(void)
{
    currentStatus.launchCorrection = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(140));

    currentStatus.launchCorrection = 120U;
    TEST_ASSERT_EQUAL_UINT8(120U, getTSLogEntry(140));

    currentStatus.launchCorrection = 0U; // restore
}

static void test_logger_bytes_141_142_export_inj_angle(void)
{
    currentStatus.injAngle = 0U;
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(141));
    TEST_ASSERT_EQUAL_UINT8(0U, getTSLogEntry(142));

    currentStatus.injAngle = 0x01F4U; // 500
    TEST_ASSERT_EQUAL_UINT8(lowByte(500U),  getTSLogEntry(141));
    TEST_ASSERT_EQUAL_UINT8(highByte(500U), getTSLogEntry(142));

    currentStatus.injAngle = 0U; // restore
}

static void test_readable_indices_96_101_export_pw5_pw8_launch_inj(void)
{
    currentStatus.PW5 = 1000U;
    TEST_ASSERT_EQUAL_INT16(1000, getReadableLogEntry(96));
    currentStatus.PW6 = 2000U;
    TEST_ASSERT_EQUAL_INT16(2000, getReadableLogEntry(97));
    currentStatus.PW7 = 3000U;
    TEST_ASSERT_EQUAL_INT16(3000, getReadableLogEntry(98));
    currentStatus.PW8 = 4000U;
    TEST_ASSERT_EQUAL_INT16(4000, getReadableLogEntry(99));
    currentStatus.launchCorrection = 75U;
    TEST_ASSERT_EQUAL_INT16(75, getReadableLogEntry(100));
    currentStatus.injAngle = 720U;
    TEST_ASSERT_EQUAL_INT16(720, getReadableLogEntry(101));
    // restore
    currentStatus.PW5 = 0U; currentStatus.PW6 = 0U;
    currentStatus.PW7 = 0U; currentStatus.PW8 = 0U;
    currentStatus.launchCorrection = 0U; currentStatus.injAngle = 0U;
}

// ========= Phase 10: startRevolutions (bytes 143-146) and runtimeStatusA (byte 147) =========

static void test_logger_bytes_143_146_export_start_revolutions(void)
{
    // Zero value
    currentStatus.startRevolutions =0UL;
    TEST_ASSERT_EQUAL_UINT8(0x00U, getTSLogEntry(143));
    TEST_ASSERT_EQUAL_UINT8(0x00U, getTSLogEntry(144));
    TEST_ASSERT_EQUAL_UINT8(0x00U, getTSLogEntry(145));
    TEST_ASSERT_EQUAL_UINT8(0x00U, getTSLogEntry(146));

    // Known multi-byte value: 0x01020304
    currentStatus.startRevolutions =0x01020304UL;
    TEST_ASSERT_EQUAL_UINT8(0x04U, getTSLogEntry(143)); // byte 0 = LSB
    TEST_ASSERT_EQUAL_UINT8(0x03U, getTSLogEntry(144));
    TEST_ASSERT_EQUAL_UINT8(0x02U, getTSLogEntry(145));
    TEST_ASSERT_EQUAL_UINT8(0x01U, getTSLogEntry(146)); // byte 3 = MSB

    currentStatus.startRevolutions =0UL; // restore
}

static void test_logger_byte_147_exports_runtime_status_a(void)
{
    // All flags clear
    currentStatus.fuelPumpOn = false;
    currentStatus.launchingHard = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.idleUpActive = false;
    TEST_ASSERT_EQUAL_UINT8(0x00U, getTSLogEntry(147));

    // fuelPumpOn only → bit 0
    currentStatus.fuelPumpOn = true;
    TEST_ASSERT_EQUAL_UINT8(0x01U, getTSLogEntry(147));

    // launchingHard only → bit 1
    currentStatus.fuelPumpOn = false;
    currentStatus.launchingHard = true;
    TEST_ASSERT_EQUAL_UINT8(0x02U, getTSLogEntry(147));

    // All four flags set → 0x0F
    currentStatus.fuelPumpOn = true;
    currentStatus.launchingHard = true;
    currentStatus.flatShiftingHard = true;
    currentStatus.idleUpActive = true;
    TEST_ASSERT_EQUAL_UINT8(0x0FU, getTSLogEntry(147));

    // restore
    currentStatus.fuelPumpOn = false;
    currentStatus.launchingHard = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.idleUpActive = false;
}

static void test_readable_indices_102_103_export_start_rev_halves(void)
{
    currentStatus.startRevolutions =0UL;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(102));
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(103));

    // Low 16 bits only
    currentStatus.startRevolutions =0x0000FFFFUL;
    TEST_ASSERT_EQUAL_INT16((int16_t)0xFFFF, getReadableLogEntry(102));
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(103));

    // High 16 bits only
    currentStatus.startRevolutions =0x00030000UL;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(102));
    TEST_ASSERT_EQUAL_INT16(3, getReadableLogEntry(103));

    currentStatus.startRevolutions =0UL; // restore
}

static void test_readable_index_104_exports_runtime_status_a(void)
{
    currentStatus.fuelPumpOn = false;
    currentStatus.launchingHard = false;
    currentStatus.flatShiftingHard = false;
    currentStatus.idleUpActive = false;
    TEST_ASSERT_EQUAL_INT16(0, getReadableLogEntry(104));

    currentStatus.flatShiftingHard = true;
    currentStatus.idleUpActive = true;
    TEST_ASSERT_EQUAL_INT16(0x0C, getReadableLogEntry(104)); // bits 2+3

    // restore
    currentStatus.flatShiftingHard = false;
    currentStatus.idleUpActive = false;
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
        // Phase 8E: new field locks (bytes 132-142)
        RUN_TEST(test_logger_bytes_132_139_export_pw5_pw8);
        RUN_TEST(test_logger_byte_140_exports_launch_correction);
        RUN_TEST(test_logger_bytes_141_142_export_inj_angle);
        // Phase 10: startRevolutions + runtimeStatusA (bytes 143-147)
        RUN_TEST(test_logger_bytes_143_146_export_start_revolutions);
        RUN_TEST(test_logger_byte_147_exports_runtime_status_a);
        // getReadableLogEntry case index locks
        RUN_TEST(test_readable_index_57_exports_status3);
        RUN_TEST(test_readable_index_58_exports_engine_protect_status);
        RUN_TEST(test_readable_index_91_exports_status5);
        RUN_TEST(test_readable_index_92_exports_knock_count);
        RUN_TEST(test_readable_index_93_exports_knock_retard);
        // Phase 8E: readable index locks (96-101)
        RUN_TEST(test_readable_indices_96_101_export_pw5_pw8_launch_inj);
        // Phase 10: readable index locks (102-104)
        RUN_TEST(test_readable_indices_102_103_export_start_rev_halves);
        RUN_TEST(test_readable_index_104_exports_runtime_status_a);
    }
}
