#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define HALL 26

Adafruit_BMP280 bmp;

const float mmPerPulse = 0.173;  //put here the value of rain in mm for each movement of the bucket

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

    Serial.print("Hall Sensor setup at pin ");
    Serial.println(HALL);

    if (!bmp.begin(0x77)) {  // You can try 0x77 if 0x76 doesn't work
        Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    }

    Serial.println("bmp was found! :D");
    Serial.println("Not in a loop!");
}

int getUVIndex()
{
    int sensorValue = analogRead(A15);
    
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

    //Serial.print("Bmp Temperature: ");
    //Serial.println(bmpTemperature);

    return bmpTemperature;
}

float getPressure()
{
    // Get pressure in hPa
    return bmp.readPressure() / 100.0F;
}

float getWindSpeed()
{
    int input = analogRead(A8); // int input = analogRead(A9);
    
    return; // TO-DO
}

float getWindDirection()
{
    int input = analogRead(A10); // int input = analogRead(A11);
    
    return; // TO-DO
}


float getHumidity()
{
    int input = analogRead(A13);
    
    return; // TO-DO
}

float getRainDensity()
{
    float temp = mmTotal;
    mmTotal = 0;

    return temp; // TO-DO
}