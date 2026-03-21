
#define HARD_REV_FIXED    1
#define HARD_REV_COOLANT  2

bool protectionTimerElapsed(uint8_t currentTime, uint8_t startTime, uint8_t delayTime);
byte checkEngineProtect(void);
byte checkRevLimit(void);
byte checkBoostLimit(void);
byte checkOilPressureLimit(void);
byte checkAFRLimit(void);
