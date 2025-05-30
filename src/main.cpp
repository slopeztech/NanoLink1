#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <st7567sfGK.h>
#include "i2c_clear_bus.h"
#include "config.h"
#include "screencontrol.h"

// Pins
const byte PinSDA = 4;
const byte PinSCL = 5;
const byte PinCE  = 9;
const byte PinCSN = 10;

// Display and screen control
st7567sfGK display;
ScreenControl lcd(&display);

// RF24 radio: CE, CSN
RF24 radio(PinCE, PinCSN);
// 5-byte (40-bit) address, defined in config.h
const uint64_t radioID = RADIO_ID;

// Radio state
bool radioListening = false;
bool debugEnabled = false;

void handleCommand(const String &cmd);
void lcd_home(const String &status = "  TX");

void setup()
{
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial) {}

  I2C_ClearBus(PinSDA, PinSCL);

  Wire.begin();
  Wire.setClock(I2C_CLOCK);
  display.begin();
  display.rotatedisplay(false);

  if (!radio.begin())
  {
    Serial.println("ERROR 1: RF24 is not responding");
    lcd_home("ERR1");
  }
  else
  {
    radio.enableDynamicPayloads();
    radio.setPALevel(PA_LEVEL);
    radio.setDataRate(DATA_RATE);
    radio.setRetries(RETRY_DELAY_STEPS, RETRY_COUNT);
    radio.setChannel(RF24_CHANNEL);

    radio.openWritingPipe(radioID);
    radio.openReadingPipe(1, radioID);
    radio.stopListening();

    Serial.println("Setup completed");
    lcd_home();
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    handleCommand(command);
  }
}

void lcd_home(const String &status)
{
  lcd.clearAll();
  lcd.setLine(0, "NanoLink 1.0 Alpha");
  lcd.setLine(1, "------------------");
  lcd.setLine(2, "Radio Mode:   " + status);
  lcd.setLine(3, "Radio Channel: " +
    String(RF24_CHANNEL >= 10 && RF24_CHANNEL <= 99 ? " " : "") +
    String(RF24_CHANNEL));
  lcd.setLine(4, "HEX Pipe:  " + String((unsigned long)radioID, HEX));
  lcd.setLine(5, "PAL, DR:      " + String(PA_LEVEL) + ", " + String(DATA_RATE));
  lcd.refresh();
}

void handleCommand(const String &cmd)
{
  if (debugEnabled) Serial.println("CMD: " + cmd);

  if (cmd == "RX")
  {
    radio.startListening();
    radioListening = true;
    lcd_home("  RX");
    if (debugEnabled) Serial.println("Switched to RX mode");
  }
  else if (cmd == "TX")
  {
    radio.stopListening();
    radioListening = false;
    lcd_home("  TX");
    if (debugEnabled) Serial.println("Switched to TX mode");
  }
  else if (cmd.startsWith("SEND "))
  {
    if (radioListening)
    {
      radio.stopListening();
      if (debugEnabled) Serial.println("Stopped listening to send");
    }

    String message = cmd.substring(5);
    if (message.length() > 31) {
      message = message.substring(0, 31);
      if (debugEnabled) Serial.println("Mensaje truncado a 31 bytes.");
    }

    bool messageSent = radio.write(message.c_str(), message.length() + 1);

    if (debugEnabled)
    {
      if (messageSent)
      {
        Serial.print("Message sent successfully: ");
        Serial.println(message.c_str());
      }
      else
      {
        Serial.println("Failed to send message");
      }
    }
  }
  else if (cmd == "PING")
  {
    Serial.println("PONG");
    lcd_home("PONG");
    delay(1500);
    lcd_home(radioListening ? "  RX" : "  TX");
  }
  else if (cmd == "DEBUG ON")
  {
    debugEnabled = true;
    Serial.println("Debug mode ENABLED");
  }
  else if (cmd == "DEBUG OFF")
  {
    debugEnabled = false;
    Serial.println("Debug mode DISABLED");
  }
  else
  {
    lcd_home("ERR2");
    if (debugEnabled) Serial.println("Unknown command");
  }
}
