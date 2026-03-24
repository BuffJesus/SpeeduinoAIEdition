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

uint8_t knockCalculateRecovery(uint8_t curKnockRetard)
{
  uint8_t tmpKnockRetard = curKnockRetard;
  //Check whether we are in knock recovery
  if((micros() - knockStartTime) > (configPage10.knock_duration * 100000UL)) //knock_duration is in seconds*10
  {
    //Calculate how many recovery steps have occurred since the
    uint32_t timeInRecovery = (micros() - knockStartTime) - (configPage10.knock_duration * 100000UL);
    uint8_t recoverySteps = timeInRecovery / (configPage10.knock_recoveryStepTime * 100000UL);
    int8_t recoveryTimingAdj = 0;
    if(recoverySteps > knockLastRecoveryStep)
    {
      recoveryTimingAdj = (recoverySteps - knockLastRecoveryStep) * configPage10.knock_recoveryStep;
      knockLastRecoveryStep = recoverySteps;
    }

    if(recoveryTimingAdj < currentStatus.knockRetard)
    {
      //Add the timing back in provided we haven't reached the end of the recovery period
      tmpKnockRetard = currentStatus.knockRetard - recoveryTimingAdj;
    }
    else
    {
      //Recovery is complete. Knock adjustment is set to 0 and we reset the knock status
      tmpKnockRetard = 0;
      BIT_CLEAR(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
      knockStartTime = 0;
      currentStatus.knockCount = 0;
    }
  }

  return tmpKnockRetard;
}

/** Ignition knock (retard) correction.
 */
int8_t knockApplyCorrection(int8_t advance)
{
  byte tmpKnockRetard = 0;

  if( (configPage10.knock_mode == KNOCK_MODE_DIGITAL)  )
  {
    //
    if(currentStatus.knockCount >= knockGetActivationCount(configPage10))
    {
      if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE))
      {
        //Knock retard is currently active already.
        tmpKnockRetard = currentStatus.knockRetard;

        //Check if additional knock events occurred
        if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE))
        {
          //Check if the latest event was far enough after the initial knock event to pull further timing
          if((micros() - knockStartTime) > (configPage10.knock_stepTime * 1000UL))
          {
            //Recalculate the amount timing being pulled
            currentStatus.knockCount++;
            tmpKnockRetard = knockCalculateRetard(currentStatus.knockCount, configPage10);
            knockStartTime = micros();
            knockLastRecoveryStep = 0;
          }
        }
        tmpKnockRetard = knockCalculateRecovery(tmpKnockRetard);
      }
      else
      {
        //Knock currently inactive but needs to be active now
        knockStartTime = micros();
        tmpKnockRetard = knockCalculateRetard(currentStatus.knockCount, configPage10);
        BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
        knockLastRecoveryStep = 0;
      }
    }

    BIT_CLEAR(currentStatus.status5, BIT_STATUS5_KNOCK_PULSE); //Reset the knock pulse indicator
  }
  else if( (configPage10.knock_mode == KNOCK_MODE_ANALOG)  )
  {
    tmpKnockRetard = currentStatus.knockRetard;

    if(BIT_CHECK(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE))
    {
      //Check if additional knock events occurred
      //Additional knock events are when the step time has passed and the voltage remains above the threshold
      if(knockEventIsValid() && ((micros() - knockStartTime) > (configPage10.knock_stepTime * 1000UL)))
      {
        //Sufficient time has passed, check the current knock value
        uint16_t tmpKnockReading = getAnalogKnock();

        if(tmpKnockReading > configPage10.knock_threshold)
        {
          currentStatus.knockCount++;

          if(currentStatus.knockCount >= knockGetActivationCount(configPage10))
          {
            tmpKnockRetard = knockCalculateRetard(currentStatus.knockCount, configPage10);
            knockStartTime = micros();
            knockLastRecoveryStep = 0;
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
          currentStatus.knockCount++;

          if(currentStatus.knockCount >= knockGetActivationCount(configPage10))
          {
            //Knock detected
            knockStartTime = micros();
            tmpKnockRetard = knockCalculateRetard(currentStatus.knockCount, configPage10);
            BIT_SET(currentStatus.status5, BIT_STATUS5_KNOCK_ACTIVE);
            knockLastRecoveryStep = 0;
          }
        }
      }
    }
  }


  tmpKnockRetard = min(tmpKnockRetard, configPage10.knock_maxRetard); //Ensure the commanded retard is not higher than the maximum allowed.
  currentStatus.knockRetard = tmpKnockRetard;
  return advance - tmpKnockRetard;
}
