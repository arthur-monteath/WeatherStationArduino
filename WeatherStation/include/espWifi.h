#include<Arduino/libraries/ESP8266WiFi>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(172, 16, 50, 10); // 172.16.50.10
EthernetServer server(80);

void setupWifi() {
  
  Ethernet.begin(mac, ip);

  server.begin();
  Serial.println(Ethernet.localIP());
}