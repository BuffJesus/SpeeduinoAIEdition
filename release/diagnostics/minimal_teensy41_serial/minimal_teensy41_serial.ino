void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("MINIMAL:SETUP");
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last >= 1000) {
    last = millis();
    Serial.println("MINIMAL:ALIVE");
  }

  while (Serial.available() > 0) {
    const int value = Serial.read();
    if (value == 'Q') {
      Serial.println("MINIMAL:Q");
    } else if (value == 'S') {
      Serial.println("MINIMAL:S");
    } else {
      Serial.print("MINIMAL:BYTE:");
      Serial.println(value);
    }
  }
}
