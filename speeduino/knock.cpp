/** @file
 * Knock detection and timing correction module.
 *
 * This module handles knock retard calculation and recovery, but NOT knock sensing.
 * Knock sensor hardware interaction remains in sensors.cpp (knockPulse ISR, getAnalogKnock, etc.)
 */

#include "knock.h"
#include "globals.h"
#include "sensors.h"
#include "timers.h"

// Global knock state instance
KnockState knockState;

// Legacy global variables for backward compatibility during migration
// TODO Phase 3: Remove these after full migration to knockState
unsigned long knockStartTime = 0;
uint8_t knockLastRecoveryStep = 0;

uint8_t knockGetActivationCount(const config10 &page10)
{
  return (page10.knock_count > 0U) ? page10.knock_count : 1U;
}

uint8_t knockCalculateRetard(uint8_t knockCount, const config10 &page10)
{
  int16_t extraSteps = (int16_t)knockCount - (int16_t)knockGetActivationCount(page10);
  if(extraSteps < 0) { extraSteps = 0; }

  // Calculate retard with overflow protection (saturate at 255)
  uint16_t retard = (uint16_t)page10.knock_firstStep + ((uint16_t)extraSteps * (uint16_t)page10.knock_stepSize);
  return (retard > 255U) ? 255U : (uint8_t)retard;
}

uint8_t knockGetStatusFlags(uint8_t knockMode, bool windowActive, uint8_t knockCount,
                             uint8_t activationCount, uint8_t retard, uint8_t lastRecoveryStep)
{
  uint8_t flags = 0;
  if (knockMode != KNOCK_MODE_OFF)  { flags |= KNOCK_STATUS_MODE_ENABLED; }
  if (windowActive)                  { flags |= KNOCK_STATUS_WINDOW_ACTIVE; }
  if (knockCount >= activationCount) { flags |= KNOCK_STATUS_COUNT_AT_THRESHOLD; }
  if (retard > 0)                    { flags |= KNOCK_STATUS_RETARD_ACTIVE; }
  if (lastRecoveryStep > 0)          { flags |= KNOCK_STATUS_RECOVERY_ACTIVE; }
  return flags;
}

uint8_t knockCalculateRecovery(uint8_t curKnockRetard)
{
  uint8_t tmpKnockRetard = curKnockRetard;
  //Check whether we are in knock recovery
  if((micros() - knockState.startTime) > (configPage10.knock_duration * 100000UL)) //knock_duration is in seconds*10
  {
    //Calculate how many recovery steps have occurred since the
    uint32_t timeInRecovery = (micros() - knockState.startTime) - (configPage10.knock_duration * 100000UL);
    uint8_t recoverySteps = timeInRecovery / (configPage10.knock_recoveryStepTime * 100000UL);
    int8_t recoveryTimingAdj = 0;
    if(recoverySteps > knockState.lastRecoveryStep)
    {
      recoveryTimingAdj = (recoverySteps - knockState.lastRecoveryStep) * configPage10.knock_recoveryStep;
      knockState.lastRecoveryStep = recoverySteps;
    }

    if(recoveryTimingAdj < knockState.retard)
    {
      //Add the timing back in provided we haven't reached the end of the recovery period
      tmpKnockRetard = knockState.retard - recoveryTimingAdj;
    }
    else
    {
      //Recovery is complete. Knock adjustment is set to 0 and we reset the knock status
      tmpKnockRetard = 0;
      BIT_CLEAR(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
      knockState.startTime = 0;
      knockState.count = 0;
    }
  }

  return tmpKnockRetard;
}

/** Ignition knock (retard) correction.
 */
int8_t knockApplyCorrection(int8_t advance)
{
  byte tmpKnockRetard = 0;

  // Sync legacy globals to knockState for backward compatibility
  knockState.startTime = knockStartTime;
  knockState.lastRecoveryStep = knockLastRecoveryStep;
  knockState.count = currentStatus.knockCount;
  knockState.retard = currentStatus.knockRetard;

  if( (configPage10.knock_mode == KNOCK_MODE_DIGITAL)  )
  {
    //
    if(knockState.count >= knockGetActivationCount(configPage10))
    {
      if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE))
      {
        //Knock retard is currently active already.
        tmpKnockRetard = knockState.retard;

        //Check if additional knock events occurred
        if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE))
        {
          //Check if the latest event was far enough after the initial knock event to pull further timing
          if((micros() - knockState.startTime) > (configPage10.knock_stepTime * 1000UL))
          {
            //Recalculate the amount timing being pulled
            knockState.count++;
            tmpKnockRetard = knockCalculateRetard(knockState.count, configPage10);
            knockState.startTime = micros();
            knockState.lastRecoveryStep = 0;
          }
        }
        tmpKnockRetard = knockCalculateRecovery(tmpKnockRetard);
      }
      else
      {
        //Knock currently inactive but needs to be active now
        knockState.startTime = micros();
        tmpKnockRetard = knockCalculateRetard(knockState.count, configPage10);
        BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
        knockState.lastRecoveryStep = 0;
      }
    }

    BIT_CLEAR(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE); //Reset the knock pulse indicator
  }
  else if( (configPage10.knock_mode == KNOCK_MODE_ANALOG)  )
  {
    tmpKnockRetard = knockState.retard;

    if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE))
    {
      //Check if additional knock events occurred
      //Additional knock events are when the step time has passed and the voltage remains above the threshold
      if(knockEventIsValid() && ((micros() - knockState.startTime) > (configPage10.knock_stepTime * 1000UL)))
      {
        //Sufficient time has passed, check the current knock value
        uint16_t tmpKnockReading = getAnalogKnock();

        if(tmpKnockReading > configPage10.knock_threshold)
        {
          knockState.count++;

          if(knockState.count >= knockGetActivationCount(configPage10))
          {
            tmpKnockRetard = knockCalculateRetard(knockState.count, configPage10);
            knockState.startTime = micros();
            knockState.lastRecoveryStep = 0;
          }
        }
      }
      tmpKnockRetard = knockCalculateRecovery(tmpKnockRetard);
    }
    else
    {
      //If not is not currently active, we read the analog pin every 30Hz
      if( BIT_CHECK(LOOP_TIMER, BIT_TIMER_30HZ) && knockEventIsValid() )
      {
        uint16_t tmpKnockReading = getAnalogKnock();

        if(tmpKnockReading > configPage10.knock_threshold)
        {
          knockState.count++;

          if(knockState.count >= knockGetActivationCount(configPage10))
          {
            //Knock detected
            knockState.startTime = micros();
            tmpKnockRetard = knockCalculateRetard(knockState.count, configPage10);
            BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
            knockState.lastRecoveryStep = 0;
          }
        }
      }
    }
  }


  tmpKnockRetard = min(tmpKnockRetard, configPage10.knock_maxRetard); //Ensure the commanded retard is not higher than the maximum allowed.
  knockState.retard = tmpKnockRetard;

  // Sync knockState back to legacy globals for backward compatibility
  knockStartTime = knockState.startTime;
  knockLastRecoveryStep = knockState.lastRecoveryStep;
  currentStatus.knockRetard = knockState.retard;
  currentStatus.knockCount = knockState.count;

  return advance - tmpKnockRetard;
}
