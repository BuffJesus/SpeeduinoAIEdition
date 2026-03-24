#ifndef INIT_HELPERS_H
#define INIT_HELPERS_H

#include <stdint.h>

/**
 * @brief Calculate required fuel in microseconds from config value
 *
 * Converts the configPage2.reqFuel value (in ms*10) to microseconds.
 *
 * @param reqFuelTenthMs Required fuel value in tenths of milliseconds (e.g., 155 = 15.5ms)
 * @return Required fuel in microseconds
 *
 * Example: reqFuelTenthMs = 155 (15.5ms) → returns 15500 µs
 */
inline uint16_t calculateReqFuelMicroseconds(uint16_t reqFuelTenthMs)
{
  return reqFuelTenthMs * 100U;
}

/**
 * @brief Calculate injector open time in microseconds from config value
 *
 * Converts the configPage2.injOpen value (in ms*10) to microseconds.
 *
 * @param injOpenTenthMs Injector open time in tenths of milliseconds (e.g., 155 = 15.5ms)
 * @return Injector open time in microseconds
 *
 * Example: injOpenTenthMs = 155 (15.5ms) → returns 15500 µs
 */
inline uint16_t calculateInjectorOpenTimeMicroseconds(uint16_t injOpenTenthMs)
{
  return injOpenTenthMs * 100U;
}

/**
 * @brief Calculate staged injection fuel multipliers
 *
 * Calculates the percentage multipliers for primary and secondary injectors
 * in staged injection configurations. These multipliers represent how much
 * longer each injector bank must pulse to deliver equivalent fuel.
 *
 * Example:
 *   Pri injectors: 250cc
 *   Sec injectors: 500cc
 *   Total capacity: 750cc
 *
 *   Pri multiplier: (100 * 750) / 250 = 300% (3x pulse width needed)
 *   Sec multiplier: (100 * 750) / 500 = 150% (1.5x pulse width needed)
 *
 * @param priInjectorSize Primary injector size in cc
 * @param secInjectorSize Secondary injector size in cc
 * @param[out] priMultiplier Primary injector percentage multiplier (e.g., 300 = 300%)
 * @param[out] secMultiplier Secondary injector percentage multiplier (e.g., 150 = 150%)
 *
 * @note Handles divide-by-zero by setting multipliers to 100 (no scaling)
 * @note Results are uint16_t to handle multipliers > 255%
 */
inline void calculateStagedInjectorMultipliers(uint16_t priInjectorSize, uint16_t secInjectorSize,
                                                uint16_t& priMultiplier, uint16_t& secMultiplier)
{
  // Prevent divide-by-zero
  if (priInjectorSize == 0 || secInjectorSize == 0)
  {
    priMultiplier = 100;
    secMultiplier = 100;
    return;
  }

  uint32_t totalInjector = (uint32_t)priInjectorSize + (uint32_t)secInjectorSize;

  // Calculate multipliers as percentages
  // Result is clamped to uint16_t range (0-65535), which represents 0-65535%
  // In practice, values > 1000% would be a severe misconfiguration
  uint32_t priCalc = (100UL * totalInjector) / priInjectorSize;
  uint32_t secCalc = (100UL * totalInjector) / secInjectorSize;

  priMultiplier = (priCalc > 65535UL) ? 65535U : (uint16_t)priCalc;
  secMultiplier = (secCalc > 65535UL) ? 65535U : (uint16_t)secCalc;
}

#endif // INIT_HELPERS_H
