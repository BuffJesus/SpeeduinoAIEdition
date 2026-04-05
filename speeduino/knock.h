#ifndef KNOCK_H
#define KNOCK_H

#include <stdint.h>
#include "globals.h"

/**
 * @brief Knock subsystem runtime state
 *
 * Encapsulates all knock-specific runtime state in a single struct.
 * This consolidates variables that were previously scattered across
 * corrections.cpp file-scope statics and currentStatus global fields.
 *
 * Phase 3: Introduced to improve module boundaries per FIRMWARE_ROADMAP.md
 * "Prefer subsystem-owned state structs for complex runtime domains"
 */
struct KnockState
{
  /** Timestamp (micros) of last knock event, used for step timing and recovery */
  unsigned long startTime;

  /** Recovery step counter, increments as recovery progresses */
  uint8_t lastRecoveryStep;

  /** Current knock retard amount in degrees (0-255) */
  uint8_t retard;

  /** Total knock events detected since last reset */
  volatile uint8_t count;

  /** Initialize knock state to safe defaults */
  void reset() volatile
  {
    startTime = 0;
    lastRecoveryStep = 0;
    retard = 0;
    count = 0;
  }
};

// Global knock state instance (defined in knock.cpp)
extern KnockState knockState;

// Knock subsystem status flags for runtime observability
// Returned by knockGetStatusFlags() as a bitmask
#define KNOCK_STATUS_MODE_ENABLED        0x01U  ///< Knock mode is not KNOCK_MODE_OFF
#define KNOCK_STATUS_WINDOW_ACTIVE       0x02U  ///< Knock detection window is open
#define KNOCK_STATUS_COUNT_AT_THRESHOLD  0x04U  ///< knockCount >= activationCount
#define KNOCK_STATUS_RETARD_ACTIVE       0x08U  ///< Timing retard > 0 is being applied
#define KNOCK_STATUS_RECOVERY_ACTIVE     0x10U  ///< Recovery is in progress (lastRecoveryStep > 0)

/**
 * @brief Get knock subsystem observable status flags
 *
 * Returns a bitmask of KNOCK_STATUS_* flags describing the current knock
 * correction state. All parameters are passed explicitly so the function
 * is pure and testable without depending on global state.
 *
 * @param knockMode Configured knock mode (KNOCK_MODE_OFF/DIGITAL/ANALOG)
 * @param windowActive Whether the knock detection window is currently open
 * @param knockCount Number of knock events detected
 * @param activationCount Threshold for knock retard activation
 * @param retard Current knock retard value (0 = no retard)
 * @param lastRecoveryStep Current recovery step counter (0 = not recovering)
 * @return uint8_t Bitmask of KNOCK_STATUS_* flags
 */
uint8_t knockGetStatusFlags(uint8_t knockMode, bool windowActive, uint8_t knockCount,
                             uint8_t activationCount, uint8_t retard, uint8_t lastRecoveryStep);

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
 * - knockState.lastRecoveryStep: Tracks recovery progress
 * - currentStatus.status5: Clears KNOCK_ACTIVE when recovery completes
 * - knockState.startTime: Reset to 0 when recovery completes
 * - knockState.count: Reset to 0 when recovery completes
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
 * - currentStatus.knockRetard: Final retard amount applied (synced from knockState.retard)
 * - currentStatus.knockCount: Knock event counter (synced from knockState.count)
 * - currentStatus.status5: KNOCK_ACTIVE and KNOCK_PULSE flags
 * - knockState.startTime: Timestamp of last knock event (persists in KnockState)
 * - knockState.lastRecoveryStep: Recovery progress tracker (persists in KnockState)
 *
 * @param advance Current ignition advance in degrees
 * @return int8_t Corrected ignition advance (advance - knockRetard)
 *
 * @note This function has extensive side effects on global state
 * @note Clamps final retard to knock_maxRetard configured limit
 */
int8_t knockApplyCorrection(int8_t advance);

#endif // KNOCK_H
