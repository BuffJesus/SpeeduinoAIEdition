#if defined(CORE_SAME51)
#include "globals.h"
#include "auxiliaries.h"

void initBoard()
{
    /*
    ***********************************************************************************************************
    * General
    */

    /*
    ***********************************************************************************************************
    * Timers
    */

    /*
    ***********************************************************************************************************
    * Auxiliaries
    */

    /*
    ***********************************************************************************************************
    * Idle
    */

    /*
    ***********************************************************************************************************
    * Schedules
    */
}

void beginBoardSerial()
{
    Serial.begin(115200);
}

void finaliseBoardTriggerSetup()
{
}

bool applyBoardPinMapping(byte boardID)
{
    (void)boardID;
    return false;
}

uint16_t freeRam()
{
  return 0;
}

void doSystemReset() { return; }
void jumpToBootloader() { return; }

#endif
