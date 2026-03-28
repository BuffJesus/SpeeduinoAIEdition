#include "globals.h"
#if defined(CORE_TEMPLATE)


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

}

void doSystemReset() { return; }
void jumpToBootloader() { return; }

#endif
