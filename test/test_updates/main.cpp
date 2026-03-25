#include <Arduino.h>
#include <unity.h>
#include <avr/sleep.h>

void testConfigMigrations(void);
void testConfigMigrations_group1(void);
void testConfigMigrations_group2(void);
void testConfigMigrations_group3(void);
void testConfigMigrations_group3a(void);
void testConfigMigrations_group3b(void);
void testConfigMigrations_group3b1(void);
void testConfigMigrations_group3b2(void);

#define UNITY_EXCLUDE_DETAILS

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

#if !defined(SIMULATOR)
    delay(2000);
#endif

    UNITY_BEGIN();

    testConfigMigrations_group1();
    testConfigMigrations_group2();
    testConfigMigrations_group3a();

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
