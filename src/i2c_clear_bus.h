#ifndef I2C_CLEAR_BUS_H
#define I2C_CLEAR_BUS_H

#include <Arduino.h>

// https://www.forward.com.au/pfod/ArduinoProgramming/I2C_ClearBus/index.html

/**
 * This routine turns off the I2C bus and clears it
 * on return SCA and SCL pins are tri-state inputs.
 * You need to call Wire.begin() after this to re-enable I2C
 * This routine does NOT use the Wire library at all.
 *
 * returns 0 if bus cleared
 *         1 if SCL held low.
 *         2 if SDA held low by slave clock stretch for > 2sec
 *         3 if SDA held low after 20 clocks.
 */
int I2C_ClearBus(byte SDA, byte SCL);

#endif // I2C_CLEAR_BUS_H
