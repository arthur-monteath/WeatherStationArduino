#include <Arduino.h>
#include <LiquidCrystal.h>

#define SELECT 0
#define LEFT   1
#define RIGHT  2
#define UP     3
#define DOWN   4
#define NONE   5

#define AHT 0 // Temperature + Humidity
#define BMP 1 // Pressure
#define UV 2  // UV Index
#define WS 3  // Wind Speed
#define WD 4  // Wind Direction
#define RG 5  // Rain Gauge

int button = 0;

const int amountOfSensors = 6;

String sensors[amountOfSensors] = {"AHT 15", "BMP 280", "UV Sensor", "Wind Speed", "Wind Direction", "Rain Gauge"};
int index = 0;

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setupLcd()
{
    // set up t/he LCD's number of columns and rows:
    lcd.begin(16, 2);
}

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

int holdingTicks = 0;
bool viewingData = false;

void updateLcdButtons()
{   
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
}