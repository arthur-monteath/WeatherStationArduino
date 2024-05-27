#include "ethernetFunctions.h"
#include "lcdController.h"
#include "sensors.h"

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  setupLcd();
  setupEthernet();
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

int ticks = 0;
void loop() {
  
  connectionLoop();

  update50ms();
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