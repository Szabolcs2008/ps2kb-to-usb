#include "hardware/watchdog.h"

void printBuffer(uint8_t const*  buffer, int size) {
  Serial.printf("{");
  for (int i = 0; i < size; i++) {
    Serial.printf(" %d", buffer[i]);
  }
  Serial.printf(" }\n");
}

bool waitPin(uint8_t pin, uint8_t expectedState, unsigned long timeout_us) {
  unsigned long start = micros();
  while (digitalRead(pin) != expectedState) {
    if (((micros() - start) > timeout_us)) return false;
  }
  return true;
}

void reset() {
  // No way to cleanly reset from software? really?
  watchdog_enable(1, 0); // 1ms 
  while (1);
}