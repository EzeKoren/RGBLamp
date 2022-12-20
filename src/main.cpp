#include <ESP8266WiFi.h>	  // Replace with WiFi.h for ESP32
#include <ESP8266WebServer.h> // Replace with WebServer.h for ESP32
#include <AutoConnect.h>

#include <LittleFS.h> 

#define RED D6
#define GREEN D5
#define BLUE D7

#define PWM_RES 256

ESP8266WebServer Server; // Replace with WebServer for ESP32
AutoConnect Portal(Server);
AutoConnectConfig acConfig;

int red;
int green;
int blue;

const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
};

void rootPage() {
	File file = LittleFS.open("index.html", "r");
	if(!file) {
		Serial.println("error opening index.html");
	} else {
		Serial.println(file.size());
	}
	Server.sendHeader("Content-Length", String(file.size()));
	Server.streamFile(file, "text/html");
	file.close();
}

void getColor() {
	String color = "rgb(" + String(red) + "," + String(green) + "," + String(blue) + ")";
	Server.sendHeader("Access-Control-Allow-Origin", "*");
	Server.send(200, "text/plain", color);
}

void api() {
	red = Server.arg(0).toInt();
	green = Server.arg(1).toInt();
	blue = Server.arg(2).toInt();

	analogWrite(RED, pgm_read_byte(&gamma8[red]));
	analogWrite(GREEN, pgm_read_byte(&gamma8[green]));
	analogWrite(BLUE, pgm_read_byte(&gamma8[blue]));

	Server.sendHeader("Access-Control-Allow-Origin", "*");
	Server.send(200, "text/plain", "ok");
}

void prepareServer() {
	Server.on("/", rootPage);
}

void setup() {
	analogWriteRange(PWM_RES);
	pinMode(RED, OUTPUT);
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);

	Serial.begin(115200);
	LittleFS.begin();
	Serial.println();

	Server.on("/api", api);
	Server.on("/getColor", getColor);

	prepareServer();

	acConfig.apid = "RGBLamp";
	Portal.config(acConfig);
	
	if (Portal.begin()) {
		Serial.println("WiFi connected: " + WiFi.localIP().toString());
		analogWrite(GREEN, 255);
		delay(250);
		analogWrite(GREEN, 0);
		delay(250);
		analogWrite(GREEN, 255);
		delay(250);
		analogWrite(GREEN, 0);
	}
}

void loop() {
	Portal.handleClient();
}
