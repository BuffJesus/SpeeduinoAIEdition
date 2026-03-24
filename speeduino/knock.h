#ifndef KNOCK_H
#define KNOCK_H

#include <stdint.h>
#include "globals.h"

// Knock state variables (defined in corrections.cpp for now)
extern unsigned long knockStartTime;
extern uint8_t knockLastRecoveryStep;

/**
 * @brief Get the knock activation count threshold
 *
 * Returns the number of knock events required before knock retard becomes active.
 * If the configured value is 0, defaults to 1 to prevent division-by-zero issues.
 *
 * @param page10 Config page 10 containing knock settings
 * @return uint8_t Knock activation threshold (1-7)
 */
uint8_t knockGetActivationCount(const config10 &page10);

/**
 * @brief Calculate knock retard amount based on knock count
 *
 * Calculates the ignition timing retard to apply based on the number of knock
 * events detected. Uses configured first step and step size with overflow protection.
 *
 * Formula: retard = firstStep + (extraSteps * stepSize)
 * where extraSteps = knockCount - activationCount
 *
 * @param knockCount Total number of knock events detected
 * @param page10 Config page 10 containing knock settings
 * @return uint8_t Knock retard in degrees (0-255, saturated)
 */
uint8_t knockCalculateRetard(uint8_t knockCount, const config10 &page10);

/**
 * @brief Calculate knock recovery progression
 *
 * Handles gradual knock timing recovery after knock event ends.
 * Recovery begins after knock_duration expires and progresses in steps
 * defined by knock_recoveryStep and knock_recoveryStepTime.
 *
 * Updates global state:
 * - knockLastRecoveryStep: Tracks recovery progress
 * - currentStatus.status5: Clears KNOCK_ACTIVE when recovery completes
 * - knockStartTime: Reset to 0 when recovery completes
 * - currentStatus.knockCount: Reset to 0 when recovery completes
 *
 * @param curKnockRetard Current knock retard value
 * @return uint8_t Updated knock retard after recovery adjustment
 *
 * @note This function has side effects on global state variables
 */
uint8_t knockCalculateRecovery(uint8_t curKnockRetard);

/**
 * @brief Apply knock timing correction to ignition advance
 *
 * Main knock correction function called from ignition timing calculation.
 * Handles both digital (knock sensor) and analog (voltage-based) knock detection modes.
 *
 * Digital mode:
 * - Monitors knock pulse flag set by knock ISR
 * - Activates retard when knockCount >= activationCount
 * - Steps retard up on additional knock events (respecting knock_stepTime)
 * - Applies gradual recovery after knock_duration expires
 *
 * Analog mode:
 * - Samples knock voltage at 30Hz when inactive
 * - Samples on-demand when active
 * - Compares to knock_threshold to detect knock
 * - Same activation/stepping/recovery logic as digital mode
 *
 * Updates global state:
 * - currentStatus.knockRetard: Final retard amount applied
 * - currentStatus.knockCount: Knock event counter
 * - currentStatus.status5: KNOCK_ACTIVE and KNOCK_PULSE flags
 * - knockStartTime: Timestamp of last knock event
 * - knockLastRecoveryStep: Recovery progress tracker
 *
 * @param advance Current ignition advance in degrees
 * @return int8_t Corrected ignition advance (advance - knockRetard)
 *
 * @note This function has extensive side effects on global state
 * @note Clamps final retard to knock_maxRetard configured limit
 */
int8_t knockApplyCorrection(int8_t advance);

#endif // KNOCK_H
