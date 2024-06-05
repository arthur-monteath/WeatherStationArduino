#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_AHTX0.h>

#define HALL 26

Adafruit_AHTX0 aht;

Adafruit_BMP280 bmp;

const float mmPerPulse = 0.173;  //put here the value of rain in mm for each movement of the bucket

const int UVPin = A3;
const int directionPin = A4;
const int speedPin = A5;

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

    if (bmp.begin()) {
        Serial.println("Found BMP280");
    } else {
        Serial.println("Didn't find BMP280");
    }

    if (aht.begin()) {
        Serial.println("Found AHT20");
    } else {
        Serial.println("Didn't find AHT20");
    }

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

float getTemperature()
{
    // Temperature in Celsius
    float bmpTemperature = bmp.readTemperature();

    Serial.print("Bmp Temperature: ");
    Serial.println(bmpTemperature);

    sensors_event_t humidity, temp;

    aht.getEvent(&humidity, &temp);

    return temp.temperature;
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
    
    return input;
}


float getHumidity()
{
    sensors_event_t humidity, temp;

    aht.getEvent(&humidity, &temp);

    return humidity.relative_humidity;
}

float getRainDensity()
{
    float temp = mmTotal;
    mmTotal = 0;

    return temp; // TO-DO
}