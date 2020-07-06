/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
// Load Wi-Fi library
#include <ESP8266WiFi.h>

const char* ssid = "Harry";
const char* password = "1234567Q";

// Set web server port number to 80
WiFiServer server(80);

String ip = "192.168.0.3";

String header;

// Auxiliar variables to store the current output state
String outputState = "off";

// Assign output variables to GPIO pins
const int output = 16;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(output, OUTPUT);
// Set outputs to LOW
  digitalWrite(output, HIGH);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// the loop function runs over and over again forever
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  
    if (client) { // If a new client connects,
   Serial.println("New Client."); // print a message out in the serial port
   String currentLine = ""; // make a String to hold incoming data from the client
   while (client.connected()) { // loop while the client's connected
   if (client.available()) { // if there's bytes to read from the client,
    char c = client.read(); // read a byte, then
    Serial.write(c); // print it out the serial monitor
    header += c;
    if (c == '\n') { // if the byte is a newline character
     // if the current line is blank, you got two newline characters in a row.
     // that's the end of the client HTTP request, so send a response:
     if (currentLine.length() == 0) {
      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
      // and a content-type so the client knows what's coming, then a blank line:
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println("Connection: close");
      client.println();

    // turns the GPIOs on and off
    if (header.indexOf("GET /5/on") >= 0) {
      Serial.println("GPIO on");
      outputState = "on";
      digitalWrite(output, LOW);
    } 
    else if (header.indexOf("GET /5/off") >= 0) {
      Serial.println("GPIO off");
      outputState = "off";
      digitalWrite(output, HIGH );
    }

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
    client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
    client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
    client.println(".button2 {background-color: #77878A;}</style></head>");
    // Web Page Title
    client.println("<h1>ESP8266 Web Server</h1>");
    client.println("<p>GPIO - State " + outputState + "</p>");
    if (outputState=="off") {
     client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
    } else {
     client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
    }
     client.println("</body></html>");
     }
    }
   }
   }
   }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
}
