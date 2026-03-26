#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>

extern void test_setup();

void setup()
{
    UNITY_BEGIN();
    test_setup();
    UNITY_END();

#if defined(SIMULATOR)
    cli();
    sleep_enable();
    sleep_cpu();
#endif
}

void loop() {}
