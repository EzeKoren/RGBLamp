#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#include <ESP8266mDNS.h>

#define RED 0
#define GREEN 2
#define BLUE 3

WiFiServer server(80);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

String header;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
	Serial.print("Assigned IP: ");
	Serial.println(WiFi.localIP());

	if (!MDNS.begin("esp8266")) {
		Serial.println("Error iniciando mDNS");
	}
	Serial.println("mDNS iniciado");
	
	server.begin();
}

void loop() {
    // put your main code here, to run repeatedly:
	WiFiClient client = server.available();
	if (client)
	{								   // If a new client connects,
		Serial.println("New Client."); // print a message out in the serial port
		String currentLine = "";	   // make a String to hold incoming data from the client
		currentTime = millis();
		previousTime = currentTime;
		while (client.connected() && currentTime - previousTime <= timeoutTime)
		{ // loop while the client's connected
			currentTime = millis();
			if (client.available())
			{							// if there's bytes to read from the client,
				char c = client.read(); // read a byte, then
				Serial.write(c);		// print it out the serial monitor
				header += c;
				if (c == '\n')
				{ // if the byte is a newline character
					// if the current line is blank, you got two newline characters in a row.
					// that's the end of the client HTTP request, so send a response:
					if (currentLine.length() == 0)
					{
						// HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
						// and a content-type so the client knows what's coming, then a blank line:
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println("Connection: close");
						client.println();

						// turns the GPIOs on and off
						// if (header.indexOf("GET /5/on") >= 0)
						// {
						// 	Serial.println("GPIO 5 on");
						// 	output5State = "on";
						// 	digitalWrite(output5, HIGH);
						// }
						// else if (header.indexOf("GET /5/off") >= 0)
						// {
						// 	Serial.println("GPIO 5 off");
						// 	output5State = "off";
						// 	digitalWrite(output5, LOW);
						// }
						// else if (header.indexOf("GET /4/on") >= 0)
						// {
						// 	Serial.println("GPIO 4 on");
						// 	output4State = "on";
						// 	digitalWrite(output4, HIGH);
						// }
						// else if (header.indexOf("GET /4/off") >= 0)
						// {
						// 	Serial.println("GPIO 4 off");
						// 	output4State = "off";
						// 	digitalWrite(output4, LOW);
						// }

						// Display the HTML web page
						client.println("<!DOCTYPE html><html>");
						client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
						client.println("<link rel=\"icon\" href=\"data:,\">");
						// CSS to style the on/off buttons
						// Feel free to change the background-color and font-size attributes to fit your preferences
						client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
						client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
						client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
						client.println(".button2 {background-color: #77878A;}</style></head>");

						// Web Page Heading
						client.println("<body><h1>ESP8266 Web Server</h1>");

						// // Display current state, and ON/OFF buttons for GPIO 5
						// client.println("<p>GPIO 5 - State " + output5State + "</p>");
						// // If the output5State is off, it displays the ON button
						// if (output5State == "off")
						// {
						// 	client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
						// }
						// else
						// {
						// 	client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
						// }

						// // Display current state, and ON/OFF buttons for GPIO 4
						// client.println("<p>GPIO 4 - State " + output4State + "</p>");
						// // If the output4State is off, it displays the ON button
						// if (output4State == "off")
						// {
						// 	client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
						// }
						// else
						// {
						// 	client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
						// }
						client.println("</body></html>");

						// The HTTP response ends with another blank line
						client.println();
						// Break out of the while loop
						break;
					}
					else
					{ // if you got a newline, then clear currentLine
						currentLine = "";
					}
				}
				else if (c != '\r')
				{					  // if you got anything else but a carriage return character,
					currentLine += c; // add it to the end of the currentLine
				}
			}
		}
		// Clear the header variable
		header = "";
		// Close the connection
		client.stop();
		Serial.println("Client disconnected.");
		Serial.println("");
	}
}
