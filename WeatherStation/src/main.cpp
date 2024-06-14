#include "lcdController.h"
#include "sensors.h"
#include<Wire.h>
#include<SoftwareSerial.h>
#include<ArduinoJson.h>

void setup() {
  
  Wire.begin();
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Serial has Begun");

  setupSensors();

  //setupLcd();
  //setupEthernet();
  //setupWifi();
}

int uvIndex = 0;
float temperature = 0.0; // Celsius
float pressure = 0.0; // hPa
float windSpeed = 0.0; // m/s
float windDirection = 0.0; // degrees
float humidity = 0.0;
float rainDensity = 0.0;

int num = 0;

void updateSensorsData()
{
  //Serial.println("Fetching Sensors...");
  
  //Serial.println("Fetching Temperature and Humidity...");
  //getTemperatureAndHumidity(&temperature, &humidity);

  //Serial.println("Fetching UV Index...");
  uvIndex = getUVIndex();
  //Serial.println("Fetching Temperature...");
  temperature = getTemperature();
  //Serial.println("Fetching Pressure...");
  pressure = getPressure();
  //Serial.println("Fetching Wind Speed...");
  windSpeed = getWindSpeed();
  //Serial.println("Fetching Wind Direction...");
  windDirection = getWindDirection();

  // Create a JSON object
  JsonDocument doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["pressure"] = pressure;
  doc["direction"] = windDirection;
  doc["speed"] = windSpeed;
  doc["uv"] = uvIndex;

  // Serialize the JSON object to a string
  String jsonString;
  serializeJson(doc, jsonString);
  Serial.println(jsonString);

  // Print the updated sensor values
  /*Serial.println("...Updated Sensors #" + num);
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
  Serial.println(" %");*/
}

/*
void handleClientRequest(EthernetClient client) {
  if (client.available()) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/data") != -1) {
      // Create a JSON object
      JsonDocument doc;
      doc["temperature"] = temperature;
      doc["humidity"] = humidity;
      doc["pressure"] = pressure;
      doc["direction"] = windDirection;
      doc["speed"] = windSpeed;
      doc["uv"] = uvIndex;

      // Serialize the JSON object to a string
      String jsonString;
      serializeJson(doc, jsonString);
      client.println(jsonString);

      // Send HTTP response with JSON content type
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.println(jsonString);
    } else {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
    }
  }
}

void connectionLoop() {
  EthernetClient client = server.available();
  if (client) {
    if (client.connected()) {
      Serial.println("Client Connected");
      handleClientRequest(client);
      client.stop();
    }
  }
}
*/

int ticks = 0;

void loop() {
  
  //connectionLoop();

  if(ticks % 5 == 0)
  {
    rainGaugeCheck();
  }

  if(ticks % 50 == 0)
  {
    
  }

  if(ticks >= 100)
  {
    updateSensorsData();
    ticks = 0;
  }

  ticks++;

  delay(100);
}