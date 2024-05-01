#include <SPI.h>
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169,254,79,196);

void setup() {
Serial.begin(9600); // disable SD card if one in the slot
pinMode(4,OUTPUT);
digitalWrite(4,HIGH);
Serial.println("Starting w5100");
Ethernet.begin(ip);
Serial.println(Ethernet.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
