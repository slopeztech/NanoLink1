// config.h - Configuration for NanoLink

#ifndef CONFIG_H
#define CONFIG_H

// Serial communication baud rate
#define SERIAL_BAUDRATE 115200

// I2C clock
#define I2C_CLOCK 400000 // 400 kHz

// 5-byte (40 bits) hexadecimal address for RF24
#define RADIO_ID  0x0001000001LL

// RF24 channel (from 0 to 125, 76 is the default)
#define RF24_CHANNEL 76

// RF24 power level (choose one of RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX)
#define PA_LEVEL   RF24_PA_MAX

// RF24 data rate (choose one of RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
#define DATA_RATE  RF24_250KBPS

// Retries settings: (delay steps, count)
//  delay steps = number of 250µs intervals before retrying
//  count = maximum number of retries
#define RETRY_DELAY_STEPS  5
#define RETRY_COUNT        15

#endif // CONFIG_H