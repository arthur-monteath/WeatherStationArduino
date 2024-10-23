#include "lcdController.h"
#include "sensors.h"
#include<espWifi.h>
#include<espSerial.h>
#include<Wire.h>
#include<SoftwareSerial.h>
#include<ArduinoJson.h>

String jsonData = ""; // Variable to store JSON data

void setup() {
  
  /*Wire.begin();
  Serial.begin(115200);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Serial has Begun");

  //setupESP();

  setupSensors();

  //setupLcd();
  //setupEthernet();*/
  setupWifi();
}

int uvIndex = 0;
float temperature = 0.0; // Celsius
float pressure = 0.0; // hPa
float windSpeed = 0.0; // m/s
float windDirection = 0.0; // degrees
float humidity = 0.0;
float rainDensity = 0.0;

int num = 0;

void debugSensors()
{
  Serial.println("...Updated Sensors #" + num);
  num++;

  Serial.print("UV Index: ");
  Serial.println(uvIndex);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Wind Speed: ");
  Serial.print(windSpeed);
  Serial.println(" m/s");

  Serial.print("Wind Direction: ");
  Serial.print(windDirection);
  Serial.println(" degrees");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void updateSensorsData()
{
  //Serial.println("Fetching UV Index...");
  uvIndex = 6;
  //Serial.println("Fetching Temperature...");
  temperature = getTemperature();
  //Serial.println("Fetching Pressure...");
  pressure = getPressure();
  //Serial.println("Fetching Wind Speed...");
  windSpeed = getWindSpeed();
  //Serial.println("Fetching Wind Direction...");
  windDirection = getWindDirection();

  humidity = 63.2;

  // Create a JSON object
  JsonDocument doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["direction"] = windDirection;
  doc["speed"] = windSpeed;
  doc["uv"] = uvIndex;

  // Serialize the JSON object to a string
  serializeJson(doc, jsonData);

  debugSensors();
}

int ticks = 0;

void loop() {

  //connectionLoop(jsonData);

  if(ticks % 5 == 0)
  {
    rainGaugeCheck();
  }

  if(ticks >= 100)
  {
    updateSensorsData();
    ticks = 0;
  }

  ticks++;

  delay(100);
}