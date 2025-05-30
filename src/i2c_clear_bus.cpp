#include "i2c_clear_bus.h"

int I2C_ClearBus(byte SDA, byte SCL) {
#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN));  // Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP);  // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power-up of the DS3231 module.

  boolean SCL_LOW = (digitalRead(SCL) == LOW);  // Check if SCL is Low.
  if (SCL_LOW) {
    return 1;  // I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // Check if SDA is Low.
  int clockCount = 20;  // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) {
    clockCount--;
    pinMode(SCL, INPUT);
    pinMode(SCL, OUTPUT);  // Clock SCL Low
    delayMicroseconds(10);
    pinMode(SCL, INPUT_PULLUP);  // Release SCL
    delayMicroseconds(10);
    SCL_LOW = (digitalRead(SCL) == LOW);
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) {
      return 2;  // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW);
  }
  if (SDA_LOW) {
    return 3;  // I2C bus error. Could not clear. SDA data line held low
  }

  pinMode(SDA, INPUT);
  pinMode(SDA, OUTPUT);  // Send an I2C Start or Repeated Start control
  delayMicroseconds(10);
  pinMode(SDA, INPUT_PULLUP);  // Send I2C STOP control
  delayMicroseconds(10);
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  return 0;  // All ok
}
