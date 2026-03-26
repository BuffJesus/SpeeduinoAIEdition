#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>
#include "test_launch_flatshift.h"

#define UNITY_EXCLUDE_DETAILS

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

#if !defined(SIMULATOR)
    delay(2000);
#endif

    UNITY_BEGIN();

    testLaunchAndFlatShift();

    UNITY_END();

#if defined(SIMULATOR)
    cli();
    sleep_enable();
    sleep_cpu();
#endif
}

void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
}
