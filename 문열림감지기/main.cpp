#include <SPI.h>
#include "WizFi250.h"

// #define DEVICEID    "v40B1DE5E31F75E2"

char ssid[] = "Android9336";       // your network SSID (name)
char pass[] = "87654321";        // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status

char pushoversite[] = "api.pushover.net";
char apitoken[] = "aexkbgnfnefd6s4dwkpjkewu3h7vzc";
char userkey [] = "ueif8hp93fq82x9ncaqse3yjiimpky";
int length;

char server[]= "api.pushover.net";
// char server[] = "api.pushingbox.com";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

boolean getIsConnected = false;

String rcvbuf;

// Initialize the Ethernet client object
WiFiClient client;

void httpRequest();
void printWifiStatus();

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  Serial.println(F("\r\nSerial Init"));

  pinMode(8, INPUT_PULLUP);

  WiFi.init();

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");

  printWifiStatus();
}

void loop() {
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    if ( c != NULL ) {
      if (rcvbuf.length() > 20)
        rcvbuf = "";
      rcvbuf += c;
      Serial.write(c);
    }
  }
  // if 5 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    if (digitalRead(8)) {
      httpRequest();
    }
  }
  rcvbuf = "";
}

// this method makes a HTTP connection to the server
void httpRequest() {
  Serial.println();
  String message = "Door opened~~!!";
  Serial.println(message);
  length = 81+message.length();

  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, 80)) {
    // Serial.println("Connecting...");

    // // send the HTTP PUT request
    // client.print(F("GET /pushingbox?devid="));
    // client.print(DEVICEID);
    // client.print(F(" HTTP/1.1\r\n"));
    // client.print(F("Host: api.pushingbox.com\r\n"));
    // client.print(F("User-Agent: Arduino\r\n"));
    // client.print(F("\r\n\r\n"));
    Serial.println("connecting to Pushover...");
    
    client.println("POST /1/messages.json HTTP/1.1");
    client.println("Host: api.pushover.net");
    client.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(length);
    client.println("\r\n");;
    client.print("token=");
    client.print(apitoken);
    client.print("&user=");
    client.print(userkey);
    client.print("&message=");
    client.print(message);
    while(client.connected())  
    {
      while(client.available())
      {
        char ch = client.read();
        Serial.write(ch);
      }
    }
    client.stop();

    // note the time that the connection was made
    lastConnectionTime = millis();
    getIsConnected = true;
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
    getIsConnected = false;
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}