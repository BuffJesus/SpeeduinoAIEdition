/**
 * @file test_adc_normalization.cpp
 * @brief Regression tests locking the 12-bit → 10-bit ADC normalization contract
 *
 * On Teensy 4.1, initADC_Teensy41() sets the hardware ADC to 12-bit mode (0-4095 range).
 * readAnalogPin() must normalize to 10-bit (0-1023) before passing values to sensor math.
 * fastMap10Bit() and all downstream sensor math assume 10-bit input.
 *
 * These tests use pure arithmetic (no hardware) to lock:
 * 1. The normalization shift: 12-bit >> 2 == 10-bit
 * 2. That fastMap10Bit() produces correct output when given normalized values
 * 3. That fastMap10Bit() produces wrong output for raw 12-bit values (documents the bug)
 */

#include <unity.h>
#include "../test_utils.h"

extern int16_t fastMap10Bit(uint16_t value, int16_t rangeMin, int16_t rangeMax);

// ========= Normalization shift math =========

static void test_adc12bit_shift_max(void)
{
    // 12-bit full scale (4095) >> 2 must equal 10-bit full scale (1023)
    TEST_ASSERT_EQUAL_UINT16(1023U, (uint16_t)(4095U >> 2U));
}

static void test_adc12bit_shift_zero(void)
{
    TEST_ASSERT_EQUAL_UINT16(0U, (uint16_t)(0U >> 2U));
}

static void test_adc12bit_shift_midscale(void)
{
    // 12-bit mid-scale (2048) >> 2 == 512 (10-bit mid-scale)
    TEST_ASSERT_EQUAL_UINT16(512U, (uint16_t)(2048U >> 2U));
}

// ========= fastMap10Bit with normalized 12-bit input =========

static void test_adc12bit_normalized_full_scale_tps(void)
{
    // 12-bit raw 4095 → normalized 1023 → TPS byte 254
    // fastMap10Bit uses >> 10 (÷1024, not ÷1023) so full-scale 1023 yields 254, not 255.
    // This is the same precision-loss documented in test_fastMap10Bit.cpp.
    TEST_ASSERT_EQUAL_INT16(254, fastMap10Bit(4095U >> 2U, 0, 255));
}

static void test_adc12bit_normalized_zero_tps(void)
{
    // 12-bit raw 0 → normalized 0 → TPS byte 0 (closed throttle)
    TEST_ASSERT_EQUAL_INT16(0, fastMap10Bit(0U >> 2U, 0, 255));
}

static void test_adc12bit_normalized_map_kpa(void)
{
    // Typical MAP range 10-400 kPa.
    // 12-bit raw 4095 → normalized 1023 → 400 kPa max
    TEST_ASSERT_EQUAL_INT16(399, fastMap10Bit(4095U >> 2U, 10, 400));
    // 12-bit raw 0 → normalized 0 → 10 kPa min
    TEST_ASSERT_EQUAL_INT16(10, fastMap10Bit(0U >> 2U, 10, 400));
}

// ========= fastMap10Bit with raw 12-bit input — documents the overrange bug =========

static void test_adc12bit_raw_overranges_tps(void)
{
    // Without normalization: raw 12-bit 4095 fed to fastMap10Bit gives wrong TPS
    // readAnalogPin() on Teensy 4.1 MUST normalize before calling fastMap10Bit
    TEST_ASSERT_NOT_EQUAL_INT16(255, fastMap10Bit(4095U, 0, 255));
}

static void test_adc12bit_raw_overranges_map(void)
{
    // Without normalization: raw 12-bit 4095 gives wrong MAP reading
    TEST_ASSERT_NOT_EQUAL_INT16(399, fastMap10Bit(4095U, 10, 400));
}

void test_adc_normalization(void)
{
    SET_UNITY_FILENAME() {
        // Shift math
        RUN_TEST(test_adc12bit_shift_max);
        RUN_TEST(test_adc12bit_shift_zero);
        RUN_TEST(test_adc12bit_shift_midscale);
        // Correct behavior with normalized input
        RUN_TEST(test_adc12bit_normalized_full_scale_tps);
        RUN_TEST(test_adc12bit_normalized_zero_tps);
        RUN_TEST(test_adc12bit_normalized_map_kpa);
        // Overrange bug without normalization
        RUN_TEST(test_adc12bit_raw_overranges_tps);
        RUN_TEST(test_adc12bit_raw_overranges_map);
    }
}
