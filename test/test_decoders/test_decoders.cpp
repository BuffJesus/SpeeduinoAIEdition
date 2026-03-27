
#include <Arduino.h>
#include <globals.h>
#include <unity.h>
#include <avr/sleep.h>

#include "missing_tooth/missing_tooth.h"
#include "Non360/Non360.h"
#include "dual_wheel/dual_wheel.h"
#include "renix/renix.h"
#include "Nissan360/Nissan360.h"
#include "FordST170/FordST170.h"
#include "BasicDistributor/BasicDistributor.h"
#include "Audi135/Audi135.h"
#include "Daihatsu/Daihatsu.h"
#include "DRZ400/DRZ400.h"
#include "FourTwentyA/FourTwentyA.h"
#include "Gm24X/Gm24X.h"
#include "Gm7X/Gm7X.h"
#include "FourG63/FourG63.h"
#include "Harley/Harley.h"
#include "Jeep2000/Jeep2000.h"
#include "MazdaAU/MazdaAU.h"
#include "Miata9905/Miata9905.h"
#include "HondaJ32/HondaJ32.h"
#include "HondaD17/HondaD17.h"
#include "Subaru67/Subaru67.h"
#include "ThirtySixMinus222/ThirtySixMinus222.h"
#include "ThirtySixMinus21/ThirtySixMinus21.h"
#include "Vmax/Vmax.h"
#include "NGC/test_ngc.h"
#include "SuzukiK6A/SuzukiK6A.h"

extern void testDecoder_General(void);
extern void testTriggerTraceReplay(void);

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
#if !defined(SIMULATOR)
    delay(2000);
#endif

    UNITY_BEGIN();    // IMPORTANT LINE!

    testDecoder_General();
    testMissingTooth();
    testNon360();
    testDualWheel();
    testRenix();
    testNissan360();
    testFordST170();
    testBasicDistributor();
    testAudi135();
    testDaihatsu();
    testDRZ400();
    testFourTwentyA();
    testGm24X();
    testGm7X();
    testFourG63();
    testHarley();
    testJeep2000();
    testMazdaAU();
    testMiata9905();
    testHondaD17();
    testHondaJ32();
    testSubaru67();
    testThirtySixMinus222();
    testVmax();
    testNGC();
    testSuzukiK6A_setEndTeeth();
    testSuzukiK6A_getCrankAngle();
    testSuzukiK6A_runtimeState();
    testTriggerTraceReplay();
    testThirtySixMinus21();
    
    UNITY_END(); // stop unit testing

#if defined(SIMULATOR)       // Tell SimAVR we are done
    cli();
    sleep_enable();
    sleep_cpu();
#endif    
}

void loop()
{
    // Blink to indicate end of test
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}
