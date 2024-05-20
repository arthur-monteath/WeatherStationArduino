#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

void setup() {
  
  Ethernet.begin(mac);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Open serial communications and wait for port to open:

  Serial.println("Part 1");

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

void loop() {
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