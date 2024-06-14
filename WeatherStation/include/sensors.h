#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include "AHT25.h"

#define HALL 8

Adafruit_BMP280 bmp;

const float mmPerPulse = 0.173;  //put here the value of rain in mm for each movement of the bucket

const int UVPin = A1;
const int directionPin = A2;
const int speedPin = A3;

float mmTotal = 0;
int sensorHall = 0;
int precedingState = 0;

void rainGaugeCheck() {
  sensorHall = digitalRead(HALL);
  
  if (sensorHall != precedingState) {
    mmTotal += mmPerPulse;
  }
  
  precedingState = sensorHall;
}

void setupSensors()
{
    pinMode(HALL, INPUT); // Setup Hall Effect

    if (bmp.begin(0x76)) {
        Serial.println("Found BMP280");
    } else {
        Serial.println("Didn't find BMP280");
    }

    /*Serial.println("Starting AHT25..."); AHT25 broke so we weren't able to use it

    if (initAHT25()) {
    Serial.println("Found AHT25");
    } else {
        Serial.println("Didn't find AHT25");
    }*/

    Serial.println("Sensors setup done!");
}

int getUVIndex()
{
    int sensorValue = analogRead(UVPin);
    
    double mV = sensorValue*5000/1023.0;

    if (mV < 50)    return 0; 
    if (mV < 272)   return 1;   
    if (mV < 363)   return 2;
    if (mV < 455)   return 3;
    if (mV < 554)   return 4;
    if (mV < 651)   return 5;
    if (mV < 745)   return 6;
    if (mV < 838)   return 7;    
    if (mV < 928)   return 8;    
    if (mV < 1027)  return 9;  
    if (mV < 1124)  return 10;     
    return 11;
}

void getTemperatureAndHumidity(float* varTemp, float* varHumidity)
{
    // Temperature in Celsius
    /*float bmpTemperature = bmp.readTemperature();

    Serial.print("Bmp Temperature: ");
    Serial.println(bmpTemperature);*/

    float humidity, temp;

    if(!readAHT25(&temp, &humidity))
    {
        Serial.println("Failed to get AHT data");
    }
    else
    {
        Serial.println("AHT Fetched");

        *varHumidity = humidity;
        *varTemp = temp;
    }
}

float getTemperature()
{
    return bmp.readTemperature();
}

float getPressure()
{
    // Get pressure in hPa
    return bmp.readPressure() / 100.0F;
}

float getWindSpeed()
{
    int input = analogRead(speedPin); // int input = analogRead(A9);
    
    return input;
}

float getWindDirection()
{
    int input = analogRead(directionPin); // int input = analogRead(A11);
    
    /*float OldRange = 1023; // 0 - 1023  
    float NewRange = 360; // 0 -360  
    float value = (input * NewRange) / OldRange;*/

    return input;
}

float getRainDensity()
{
    float temp = mmTotal;
    mmTotal = 0;

    return temp; // TO-DO
}