
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(169,254,79,196); //<<< ENTER YOUR IP ADDRESS HERE!!!

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

int buttonPress = 1;

void setup()
{
  Serial.begin(9600);
  
  pinMode(2, INPUT);

  Serial.println("About to begin");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  Serial.println(Ethernet.localIP());
}

void loop()
{
  buttonPress = digitalRead(2); 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          //serves a different version of a website depending on whether or not the button
          //connected to pin 2 is pressed.
          if (buttonPress == 1) {
            client.println("<cke:html><cke:body bgcolor=#FFFFFF>LIGHT!</cke:body></cke:html>");
          }
          else if (buttonPress == 0){
            client.println("<cke:html><cke:body bgcolor=#000000 text=#FFFFFF>DARK!</cke:body></cke:html>");
          }
          

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
