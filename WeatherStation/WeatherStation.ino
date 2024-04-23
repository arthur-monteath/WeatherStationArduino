/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

#define SELECT 0
#define LEFT   1
#define RIGHT  2
#define UP     3
#define DOWN   4
#define NONE   5

#define AHT 0
#define BMP 1
#define UV 2
#define WS 3
#define WD 4
#define RG 5

int button = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //Serial.begin(9600);
}

const int amountOfSensors = 6;
String sensors[amountOfSensors] = {"AHT 15", "BMP 280", "UV Sensor", "Wind Speed", "Wind Direction", "Rain Gauge"};
int index = 0;

int readLcdButtons()
{
  int input = analogRead(A0);
  
  if (input > 1000) return NONE;    // 1021
  if (input > 550)   return SELECT; // 637
  if (input > 300)  return LEFT;    // 408
  if (input > 200)  return DOWN;    // 255
  if (input > 50)  return UP;       // 97
  return RIGHT;                     // 0
}

int uvIndex = 0;
void updateUVIndex()
{
  int sensorValue = analogRead(A15);

  double mV = sensorValue*5000/1023.0;

  if (mV < 50)         uvIndex = 0; 
  else if (mV < 272)   uvIndex = 1;   
  else if (mV < 363)   uvIndex = 2;
  else if (mV < 455)   uvIndex = 3;
  else if (mV < 554)   uvIndex = 4;
  else if (mV < 651)   uvIndex = 5;
  else if (mV < 745)   uvIndex = 6;
  else if (mV < 838)   uvIndex = 7;    
  else if (mV < 928)   uvIndex = 8;    
  else if (mV < 1027)  uvIndex = 9;  
  else if (mV < 1124)  uvIndex = 10;     
  else                 uvIndex = 11;                     
}

double temperature = 0.0; // Celsius
void updateTemperature()
{
  temperature = 0.0;
}

double pressure = 0.0; // hPa
void updatePressure()
{
  pressure = 0.0;
}

double windSpeed = 0.0; // m/s
void updateWindSpeed()
{
  windSpeed = 0.0;
}

double windDirection = 0.0; // degrees
void updateWindDirection()
{
  windDirection = 0.0;
}

double humidity = 0.0;
void updateHumidity()
{
  humidity = 0.0;
}

double rainDensity = 0.0;
void updateRainDensity()
{
  rainDensity = 0.0;
}

void updateSensors()
{
  updateUVIndex();
  updateTemperature();
  updatePressure();
  updateWindSpeed();
  updateWindDirection();
  updateHumidity();
}

int holdingTicks = 0;
bool viewingData = false;

void loop() {
  // set the cursor to column 0, line 0
  // (note: counting begins with 0):

  updateSensors();
  
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
     
  lcd.setCursor(0, 1);
  switch (readLcdButtons())
  {
    case LEFT:
    { 
      if(holdingTicks == 0 || holdingTicks > 10)

      holdingTicks++;
      break;
    }
    case RIGHT:
    {
      if(holdingTicks == 0 || holdingTicks > 10)

      holdingTicks++;
      break;
    }
    case UP:
    {
      if(holdingTicks == 0 || holdingTicks > 10) 
      index = index - 1 >= 0 ? index - 1 : index;

      holdingTicks++;
      break;
    }
    case DOWN:
    {
      if(holdingTicks == 0 || holdingTicks > 10)
      index = index + 1 < amountOfSensors ? index + 1 : index;
      
      holdingTicks++;
      break;
    }
    case SELECT:
    {
      if(holdingTicks == 0)
      {
        viewingData = !viewingData; lcd.clear();
      }

      holdingTicks++;
      break;
    }
    case NONE:
    {
      holdingTicks = 0;
      break;
    }
  }

  delay(50);
}
