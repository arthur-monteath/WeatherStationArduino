#include <Arduino.h>

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

double getTemperature()
{
    int input = analogRead(A14);
    
    return; // TO-DO
}

double getPressure()
{
    int input = analogRead(A12);
    
    return; // TO-DO
}

double getWindSpeed()
{
    int input = analogRead(A8); // int input = analogRead(A9);
    
    return; // TO-DO
}

double getWindDirection()
{
    int input = analogRead(A10); // int input = analogRead(A11);
    
    return; // TO-DO
}


double getHumidity()
{
    int input = analogRead(A13);
    
    return; // TO-DO
}

double getRainDensity()
{
    int input = analogRead(0);

    return; // TO-DO
}