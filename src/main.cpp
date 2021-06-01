#include <Arduino.h>

#include <EEPROM.h>

#include <Config.h>
#include <ControllerSetup.h>

int address = 0;
byte value;

ControllerSetup controllerSetup;
// OverTheAirUpdate ota;
// AwtrixWiFi wifi;
// MQTT mqtt;
// AwtrixUDP udp;
// AwtrixBlynk ESPblynk;
// AwtrixSound sound;
// ApplicationManager &applications = ApplicationManager::getInstance();
// AwtrixSettings &settings = AwtrixSettings::getInstance();

void setup()
{
  Serial.begin(115200);
  EEPROM.begin(512);

  // // write a 0 to all 512 bytes of the EEPROM
  // for (int i = 0; i < 512; i++)
  // {
  //   EEPROM.write(i, 0);
  // }

  // // turn the LED on when we're done
  // pinMode(13, OUTPUT);
  // digitalWrite(13, HIGH);

  controllerSetup.setup();
}

void loop()
{
  // read a byte from the current address of the EEPROM
  value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();

  // advance to the next address of the EEPROM
  address = address + 1;

  // there are only 512 bytes of EEPROM, from 0 to 511, so if we're
  // on address 512, wrap around to address 0
  if (address == 512)
  {
    address = 0;
  }

  delay(500);
}