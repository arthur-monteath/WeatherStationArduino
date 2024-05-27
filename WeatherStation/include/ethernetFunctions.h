#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(172, 16, 50, 10); // 172.16.50.10
EthernetServer server(80);

void setupEthernet() {
  
  Ethernet.begin(mac, ip);

  server.begin();
  Serial.println(Ethernet.localIP());
}

void handleClientRequest(EthernetClient client) {
  if (client.available()) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/data") != -1) {
      // Create a JSON object
      JsonDocument doc;
      doc["temperature"] = 25;
      doc["humidity"] = 50;
      doc["direction"] = 25;
      doc["pressure"] = 50;
      doc["speed"] = 50;
      doc["uv"] = 50;

      // Serialize the JSON object to a string
      String jsonString;
      serializeJson(doc, jsonString);
      
      // Send HTTP response with JSON content type
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Connection: close");
      client.println();
      client.println(jsonString);
    } else {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Connection: close");
    }
  }
}

void connectionLoop() {
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client Exists");
    if (client.connected()) {
      Serial.println("Client Connected");
      handleClientRequest(client);
      client.stop();
    }
  }
}