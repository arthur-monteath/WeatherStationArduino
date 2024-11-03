#include <Wire.h>
#include <ArduinoJson.h>
#include "WifiEsp.h"
#include "sensors.h"

void setup() {
  
  Wire.begin();
  Serial.begin(115200);
  
  while (!Serial)
  {
    ;
  }

  Serial.println("Serial has Begun");

  setupSensors();

  setupWifi();

  // SDA: 21 SCL: 22
}

int uvIndex = 0;
float temperature = 0.0; // Celsius
float pressure = 0.0; // hPa
float windSpeed = 0.0; // m/s
float windDirection = 0.0; // degrees
float humidity = 0.0;
float rainDensity = 0.0;

void updateSensorsData()
{
  Serial.print("Fetching UV Index... ");
  uvIndex = 6;
  Serial.println(uvIndex);

  Serial.print("Fetching Temperature... ");
  temperature = getTemperature();
  Serial.println(temperature);
  
  Serial.print("Fetching Pressure... ");
  pressure = getPressure();
  Serial.println(pressure);

  Serial.print("Fetching Wind Speed... ");
  windSpeed = getWindSpeed();
  Serial.println(windSpeed);

  Serial.print("Fetching Wind Direction... ");
  windDirection = getWindDirection();
  Serial.println(windDirection);

  humidity = 63.2;

  Serial.print("Fetching Rain Density... ");
  rainDensity = getRainDensity();
  Serial.println(rainDensity);

  // Create a JSON object
  JsonDocument doc;
  doc["temperature"] = temperature;
  //doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["direction"] = windDirection;
  doc["speed"] = windSpeed;
  doc["rain"] = rainDensity;
  doc["uv"] = uvIndex;

  uploadData(doc);
}

int ticks = 0;

void loop() {

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
