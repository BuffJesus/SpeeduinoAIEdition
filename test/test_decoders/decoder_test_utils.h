#pragma once

#include <decoders.h>

extern volatile unsigned long curTime;
extern volatile unsigned long curGap;
extern volatile unsigned long curTime2;
extern volatile unsigned long curGap2;
extern volatile unsigned long curTime3;
extern volatile unsigned long curGap3;
extern volatile unsigned long lastGap;
extern volatile unsigned long targetGap;
extern volatile unsigned long targetGap2;
extern volatile unsigned long targetGap3;
extern volatile unsigned long secondaryLastToothTime;
extern volatile unsigned long secondaryLastToothTime1;
extern volatile unsigned long toothLastThirdToothTime;
extern volatile unsigned long thirdLastToothTime;
extern volatile unsigned long thirdLastToothTime1;

static inline void resetDecoderTestTransientState(void)
{
    curTime = 0U;
    curGap = 0U;
    curTime2 = 0U;
    curGap2 = 0U;
    curTime3 = 0U;
    curGap3 = 0U;
    lastGap = 0U;
    targetGap = 0U;
    targetGap2 = 0U;
    targetGap3 = 0U;
    secondaryLastToothTime = 0U;
    secondaryLastToothTime1 = 0U;
    toothLastThirdToothTime = 0U;
    thirdLastToothTime = 0U;
    thirdLastToothTime1 = 0U;
    testSetLastSecondaryToothTime(0U);
    testClearTriggerStateOverrides();
    resetDecoder();
}
