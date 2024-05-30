#include "ethernetFunctions.h"
#include "lcdController.h"
#include "sensors.h"
#include "espWifi.h"

void setup() {

  setupSensors();

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  setupLcd();
  //setupEthernet();
  setupWifi();
}

int uvIndex = 0;
double temperature = 0.0; // Celsius
double pressure = 0.0; // hPa
double windSpeed = 0.0; // m/s
double windDirection = 0.0; // degrees
double humidity = 0.0;
double rainDensity = 0.0;

void updateSensorsData()
{
  uvIndex = getUVIndex();
  temperature = getTemperature();
  pressure = getPressure();
  windSpeed = getWindSpeed();
  windDirection = getWindDirection();
  humidity = getHumidity();
}

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

void update50ms()
{
  updateSensorsData();
  
  if(viewingData)
  {
    lcd.setCursor(0, 0);
    lcd.print("Viewing " + sensors[index] + "                 ");

    lcd.setCursor(0,1);
        
    switch(index)
    {
      case AHT: lcd.print(temperature); lcd.setCursor(7, 1); lcd.print(humidity);
      break;

      case BMP: lcd.print(pressure);
      break;

      case UV: lcd.print(uvIndex);
      break;

      case WS: lcd.print(windSpeed);
      break;

      case WD: lcd.print(windDirection);
      break;

      case RG: lcd.print(rainDensity);
      break;
    }
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print(">" + sensors[index] + "                ");
    
    lcd.setCursor(0, 1);
    if(index+1 < amountOfSensors)
    {
      lcd.print(" " + sensors[index+1] + "                ");
    }
    else
    {
      lcd.print("                "); // 16 spaces
    }
  }

  updateLcdButtons();
}

int ticks = 0;
void loop() {
  Serial.println(digitalRead(HALL)); 
  connectionLoop();

  //update50ms();
}