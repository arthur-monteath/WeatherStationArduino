#include <ESP8266WiFi.h>

const char* ssid = "EAR_WiFi";
const char* password = "Ear@2020";

// Static IP configuration
IPAddress local_IP(172, 16, 23, 202); // 172.16.23.202:80
IPAddress gateway(172, 16, 20, 1);
IPAddress subnet(255, 255, 248, 0);
IPAddress dns(8, 8, 8, 8);

WiFiServer server(80);
String jsonData = ""; // Variable to store received JSON data

void setup() {
  Serial.begin(9600); 
  Serial.println();

  // Configuring static IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.accept();

  /*if (Serial.available() > 0) {
    jsonData = Serial.readStringUntil('\n');
    Serial.println("Data received from Arduino: " + jsonData);
  }*/

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.println(jsonData);

            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}