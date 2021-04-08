#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino

//WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

// Firebase
#include <FirebaseESP8266.h>

void setup()
{
  Serial.begin(115200);

  /*
    WiFiManager
  */
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  // wifiManager.resetSettings();

  //set custom ip for portal
  //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "bcn+"
  //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("bcn+");

  //if you get here you have connected to the WiFi
  Serial.println("Device Connected! :P");
}

void loop()
{
  // put your main code here, to run repeatedly:
}