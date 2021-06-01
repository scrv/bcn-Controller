#include "ControllerSetup.h"

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

//WiFiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

HTTPClient http;
String mainURL = "http://192.168.100.19:5000/";
String controllerSerialNo;

void connect_wifi()
{
    Serial.println();
    Serial.println("--- WI-FI ---");
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //reset saved settings
    // wifiManager.resetSettings();

    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("bcn+");

    //if you get here you have connected to the WiFi
    Serial.println("Connected");
    Serial.println();
}

void resolve_host()
{
    Serial.println();
    Serial.println("--- HOST ---");

    String url = mainURL + "ping";
    http.begin(url);

    int httpCode = http.GET(); //Send the request

    if (httpCode > 0)
    { 
        //Check the returning code
        String payload = http.getString(); //Get the request response payload
        Serial.println(payload);           //Print the response payload
    }
    else
        Serial.println("An error ocurred");

    http.end(); //Close connection
    Serial.println();
}

void write_serial_no(char add, String data)
{
    int _size = data.length();
    int i;
    for (i = 0; i < _size; i++)
    {
        EEPROM.write(add + i, data[i]);
    }
    EEPROM.write(add + _size, '\0'); //Add termination null character for String Data
    EEPROM.commit();
}

String read_string(char add)
{
    char data[100]; //Max 100 Bytes
    int len = 0;
    unsigned char k;
    k = EEPROM.read(add);
    while (k != '\0' && len < 500) //Read until null character
    {
        k = EEPROM.read(add + len);
        data[len] = k;
        len++;
    }
    data[len] = '\0';
    return String(data);
}

void register_device()
{
    Serial.println();
    Serial.println("--- REGISTER ---");

    http.useHTTP10(true);

    String url = mainURL + "controllers/register";
    http.begin(url);

    int httpCode = http.POST({}); //Send the request

    if (httpCode > 0)
    {
        String payload = http.getString(); //Get the request response payload

        DynamicJsonDocument doc(512);
        DeserializationError error = deserializeJson(doc, payload);
        if (error)
            Serial.println(error.f_str());

        controllerSerialNo = doc["serial_number"].as<const char *>();
        write_serial_no(0, controllerSerialNo);

        Serial.println(controllerSerialNo);
    }
    else
        Serial.println("An error ocurred");

    http.useHTTP10(false);
    http.end(); //Close connection
}

void fetch_controller()
{
    Serial.println("--- DEVICE ---");

    controllerSerialNo = read_string(0);

    Serial.print("Serial No: ");
    if (controllerSerialNo)
    {
        Serial.println("Missing");
        register_device();
    }
    else
        Serial.println(controllerSerialNo);

    Serial.println();
}

void ControllerSetup::setup()
{
    connect_wifi();
    resolve_host();
    fetch_controller();
}