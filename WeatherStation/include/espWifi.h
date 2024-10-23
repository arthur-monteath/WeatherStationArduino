#include <WiFi.h>
#include <HttpClient.h>

char* ssid = "EAR_WiFi";             // Your Wi-Fi SSID
const char* password = "Ear@2020";   // Your Wi-Fi Password

const char* serverUrl = "http://localhost:3000/data"; // Node.js server URL
const int serverPort = 3000;

WiFiServer server(80);

void setupWifi() {
  Serial.begin(115200);
  delay(1000);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");

  // Create a WiFiClient object
  WiFiClient client;

  // Connect to the server
  if (client.connect(serverUrl, serverPort)) {
    Serial.println("Connected to server!");

    // Create the JSON payload
    String jsonPayload = "{\"data\":\"Hello from ESP32!\"}";  // Dummy JSON data

    // Send the HTTP POST request
    client.println("POST /data HTTP/1.1");
    client.println("Host: " + String(serverUrl));
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonPayload.length());
    client.println();  // Empty line to end headers
    client.println(jsonPayload);  // Send JSON payload

    // Wait for server response
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);  // Print server response
      }
    }

    // Disconnect
    client.stop();
  } else {
    Serial.println("Connection to server failed.");
  }
}
