#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,10,0, 177);

void setup() {
   Serial.begin(9600);

   // disable w5100 while setting up SD
   pinMode(10,OUTPUT);
   digitalWrite(10,HIGH);

   if(SD.begin(4) == 0) Serial.println("SD fail");
   else Serial.println("SD ok");

   Ethernet.begin(mac,ip);
   digitalWrite(10,HIGH);

   Serial.print("server is at ");
   Serial.println(Ethernet.localIP());
}

void loop() {

}
