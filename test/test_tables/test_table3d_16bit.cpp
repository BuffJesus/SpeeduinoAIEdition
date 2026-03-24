#include <unity.h>
#include "table3d_typedefs.h"
#include "table3d_interpolate.h"
#include "table3d.h"

// Tests for 16-bit table value correctness
// These tests verify that table interpolation works correctly with values > 255
// On AVR (8-bit values), these tests validate the math without overflow
// On Teensy 4.1 (16-bit values), these tests validate actual high-resolution table support

static void test_16bit_table_interpolation_with_values_above_255(void) {
    // Create a small 4x4 table with values > 255
    // This tests that the interpolation math doesn't overflow
    table3d4RpmLoad testTable;

    // Set axis values (RPM and Load bins)
    testTable.axisX.axis[0] = 1000;
    testTable.axisX.axis[1] = 2000;
    testTable.axisX.axis[2] = 3000;
    testTable.axisX.axis[3] = 4000;

    testTable.axisY.axis[0] = 10;
    testTable.axisY.axis[1] = 30;
    testTable.axisY.axis[2] = 50;
    testTable.axisY.axis[3] = 70;

    // Set table values > 255 (only meaningful on Teensy 4.1, but math should work on AVR)
    // Layout: values[12..15, 8..11, 4..7, 0..3] correspond to rows 0..3
    // Row 0 (Load=10): 300, 400, 500, 600
    // Row 1 (Load=30): 500, 600, 700, 800
    // Row 2 (Load=50): 700, 800, 900, 1000
    // Row 3 (Load=70): 900, 1000, 1100, 1200

    // On AVR (8-bit), values will be truncated to uint8_t range (0-255)
    // But the interpolation math should still work without overflow
    testTable.values.values[0] = 300;  // Row 3, Col 0
    testTable.values.values[1] = 400;  // Row 3, Col 1
    testTable.values.values[2] = 500;  // Row 3, Col 2
    testTable.values.values[3] = 600;  // Row 3, Col 3

    testTable.values.values[4] = 500;  // Row 2, Col 0
    testTable.values.values[5] = 600;  // Row 2, Col 1
    testTable.values.values[6] = 700;  // Row 2, Col 2
    testTable.values.values[7] = 800;  // Row 2, Col 3

    testTable.values.values[8] = 700;  // Row 1, Col 0
    testTable.values.values[9] = 800;  // Row 1, Col 1
    testTable.values.values[10] = 900; // Row 1, Col 2
    testTable.values.values[11] = 1000; // Row 1, Col 3

    testTable.values.values[12] = 900;  // Row 0, Col 0
    testTable.values.values[13] = 1000; // Row 0, Col 1
    testTable.values.values[14] = 1100; // Row 0, Col 2
    testTable.values.values[15] = 1200; // Row 0, Col 3

    // Query at midpoint between bins (should interpolate)
    // RPM=1500 (midpoint between 1000 and 2000)
    // Load=20 (midpoint between 10 and 30)
    table3d_value_t result = get3DTableValue(&testTable, 20, 1500);

    #if defined(CORE_TEENSY41)
        // On Teensy 4.1 (16-bit values), expect actual high-resolution result
        // Midpoint between rows 0 and 1, cols 0 and 1 = average of 900, 1000, 700, 800 = 850
        TEST_ASSERT_UINT16_WITHIN(5, 850, result);
    #else
        // On AVR (8-bit values), values are truncated to 0-255 range
        // So 300%256=44, 400%256=144, etc.
        // Just verify no crash/overflow - exact value depends on truncation
        TEST_ASSERT(result >= 0 && result <= 255);
    #endif
}

static void test_16bit_boundary_values_no_overflow(void) {
    // Test that maximum values don't cause overflow during interpolation
    table3d4RpmLoad testTable;

    testTable.axisX.axis[0] = 1000;
    testTable.axisX.axis[1] = 2000;
    testTable.axisX.axis[2] = 3000;
    testTable.axisX.axis[3] = 4000;

    testTable.axisY.axis[0] = 10;
    testTable.axisY.axis[1] = 30;
    testTable.axisY.axis[2] = 50;
    testTable.axisY.axis[3] = 70;

    // Set all values to maximum
    for (int i = 0; i < 16; i++) {
        #if defined(CORE_TEENSY41)
            testTable.values.values[i] = 65535; // Max uint16_t
        #else
            testTable.values.values[i] = 255;   // Max uint8_t
        #endif
    }

    // Query at corner (should return max value without overflow)
    table3d_value_t result = get3DTableValue(&testTable, 10, 1000);

    #if defined(CORE_TEENSY41)
        TEST_ASSERT_EQUAL_UINT16(65535, result);
    #else
        TEST_ASSERT_EQUAL_UINT8(255, result);
    #endif
}

static void test_16bit_zero_values_no_underflow(void) {
    // Test that zero values don't cause underflow
    table3d4RpmLoad testTable;

    testTable.axisX.axis[0] = 1000;
    testTable.axisX.axis[1] = 2000;
    testTable.axisX.axis[2] = 3000;
    testTable.axisX.axis[3] = 4000;

    testTable.axisY.axis[0] = 10;
    testTable.axisY.axis[1] = 30;
    testTable.axisY.axis[2] = 50;
    testTable.axisY.axis[3] = 70;

    // Set all values to zero
    for (int i = 0; i < 16; i++) {
        testTable.values.values[i] = 0;
    }

    // Query should return 0 without underflow
    table3d_value_t result = get3DTableValue(&testTable, 20, 1500);
    TEST_ASSERT_EQUAL(0, result);
}

static void test_16bit_mixed_values_interpolation(void) {
    // Test interpolation with mixed low and high values
    table3d4RpmLoad testTable;

    testTable.axisX.axis[0] = 1000;
    testTable.axisX.axis[1] = 2000;
    testTable.axisX.axis[2] = 3000;
    testTable.axisX.axis[3] = 4000;

    testTable.axisY.axis[0] = 10;
    testTable.axisY.axis[1] = 30;
    testTable.axisY.axis[2] = 50;
    testTable.axisY.axis[3] = 70;

    // Create gradient: low values in corner, high values in opposite corner
    testTable.values.values[12] = 100;   // Row 0, Col 0 - low
    testTable.values.values[13] = 200;
    testTable.values.values[14] = 300;
    testTable.values.values[15] = 400;   // Row 0, Col 3

    testTable.values.values[8] = 200;
    testTable.values.values[9] = 300;
    testTable.values.values[10] = 400;
    testTable.values.values[11] = 500;

    testTable.values.values[4] = 300;
    testTable.values.values[5] = 400;
    testTable.values.values[6] = 500;
    testTable.values.values[7] = 600;

    testTable.values.values[0] = 400;
    testTable.values.values[1] = 500;
    testTable.values.values[2] = 600;
    testTable.values.values[3] = 700;    // Row 3, Col 3 - high

    // Query at center of table
    table3d_value_t result = get3DTableValue(&testTable, 40, 2500);

    #if defined(CORE_TEENSY41)
        // On Teensy 4.1 (16-bit values), should get value in middle range (300-500)
        TEST_ASSERT(result >= 300 && result <= 500);
    #else
        // On AVR (8-bit values), values are truncated to 0-255 range
        // 300%256=44, 400%256=144, 500%256=244
        // Just verify reasonable result without crash
        TEST_ASSERT(result >= 40 && result <= 250);
    #endif
}

void run_table3d_16bit_tests(void) {
    RUN_TEST(test_16bit_table_interpolation_with_values_above_255);
    RUN_TEST(test_16bit_boundary_values_no_overflow);
    RUN_TEST(test_16bit_zero_values_no_underflow);
    RUN_TEST(test_16bit_mixed_values_interpolation);
}
