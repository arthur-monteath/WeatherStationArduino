#include <Ethernet.h>
#include <SPI.h>

// Undefine this to get a working program.
#define TELNET

// the media access control (ethernet hardware) address for the Arduino:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x92, 0x34 };

// network configuration - all except mac are optional.
// If the actual values aren't supplied, the default
// is the local_ip with last octet 1.
// void EthernetClass::begin(uint8_t *mac, IPAddress local_ip,
//   IPAddress dns_server, IPAddress gateway, IPAddress subnet)

// The default Arduino board address:
byte ip[] = { 192, 168, 1, 20 };
// The DNS server address:
byte dnss[] = { 192, 168, 1, 1 };
// the gateway router address:
byte gateway[] = { 192, 168, 1, 1 };
// the subnet mask:
byte subnet[] = { 255, 255, 255, 0 };

// telnet defaults to port 23
EthernetServer server(23);

void setup()
{
  // Initialise the serial device.
  Serial.begin(9600);
  delay(2000);
  Serial.println("setup()");

  // Disable SD SPI
  // Is this needed ???
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  // attempt a DHCP connection:
  Serial.println("Attempting to get an IP address using DHCP");
  // This requires the DHCP server to be x.x.x.1
  if (Ethernet.begin(mac) == 0) {
    // if DHCP fails, start with a hard-coded address:
    Serial.println("failed to get an IP address using DHCP, trying manually");
    Ethernet.begin(mac, ip); // function returns void
    //Ethernet.begin(mac, ip, gateway, gateway, subnet); // function returns void
  }
  else
    Serial.println("got an IP address using DHCP");

  // Start listening for clients.
  server.begin();
  // Say who we think we are.
  Serial.println(Ethernet.localIP());
}

void loop()
{
  Serial.println("loop()");

  // Initialise the client each pass ???
  EthernetClient client = server.available();

  // Read bytes from the incoming client and write them back
  // to any clients connected to the server.
#ifdef TELNET
  if ( client ) {
    char c = client.read();
    server.write(c);
  }
#endif

  // Can help debugging.
  delay(2000);
}
