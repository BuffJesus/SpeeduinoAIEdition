/** @file
 * @brief Teensy 4.1 12-bit ADC backend with higher-resolution sampling
 *
 * Teensy 4.1 supports 12-bit ADC (0-4095 range) vs standard Arduino 10-bit (0-1023).
 * This module provides:
 * - 12-bit ADC initialization during board setup
 * - Backward-compatible 10-bit scaling (default) to avoid breaking existing sensor math
 * - Opt-in native 12-bit readings for sensors that benefit from higher precision
 *
 * Implementation strategy:
 * - By default, all analogRead() results are scaled down to 10-bit (>> 2) for compatibility
 * - Specific sensors (MAP, baro, O2) can opt into native 12-bit via dedicated functions
 * - Existing sensor code requires no changes (works with 10-bit scaled values)
 */

#pragma once

#if defined(CORE_TEENSY41)

#include <Arduino.h>

/**
 * @brief Initialize Teensy 4.1 ADC to 12-bit resolution
 *
 * Called during board initialization (init.cpp under CORE_TEENSY41 guard).
 * Sets ADC hardware to 12-bit mode but scales down to 10-bit by default for compatibility.
 */
inline void initADC_Teensy41(void) {
    analogReadResolution(12); // Set hardware to 12-bit mode (0-4095 range)
}

/**
 * @brief Read ADC in backward-compatible 10-bit mode
 *
 * This is the default ADC read function used throughout the codebase.
 * Returns 10-bit scaled value (0-1023) even though hardware is in 12-bit mode.
 *
 * @param pin Analog pin to read
 * @return 10-bit ADC value (0-1023)
 */
inline uint16_t analogRead10bit(uint8_t pin) {
    return analogRead(pin) >> 2; // Scale 12-bit (0-4095) down to 10-bit (0-1023)
}

/**
 * @brief Read ADC in native 12-bit mode
 *
 * Opt-in function for sensors that benefit from higher precision (MAP, baro, O2).
 * Returns full 12-bit resolution (0-4095).
 *
 * NOTE: Caller must handle 12-bit scaling in sensor math.
 *
 * @param pin Analog pin to read
 * @return 12-bit ADC value (0-4095)
 */
inline uint16_t analogRead12bit(uint8_t pin) {
    return analogRead(pin); // Native 12-bit read (hardware already in 12-bit mode)
}

/**
 * @brief Convert 12-bit ADC reading to 10-bit equivalent
 *
 * Helper for gradual migration: allows reading in 12-bit then converting back to 10-bit.
 * Useful when testing 12-bit ADC without changing sensor math.
 *
 * @param adc12bit 12-bit ADC value (0-4095)
 * @return 10-bit ADC value (0-1023)
 */
inline uint16_t adc12to10(uint16_t adc12bit) {
    return adc12bit >> 2;
}

/**
 * @brief Convert 10-bit ADC value to 12-bit equivalent
 *
 * Helper for migrating sensor code to 12-bit: scales legacy 10-bit constants/thresholds.
 *
 * @param adc10bit 10-bit ADC value (0-1023)
 * @return 12-bit ADC value (0-4092, shifted left by 2)
 */
inline uint16_t adc10to12(uint16_t adc10bit) {
    return adc10bit << 2;
}

#endif // CORE_TEENSY41
