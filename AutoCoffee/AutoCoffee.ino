#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "credentials.h"
#include "index.h"

const char *ssid = SSID;
const char *password = PASSWORD;

ESP8266WebServer server(80);

const int led = 13;

bool makingCoffee = false;

void handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266"))
    {
        Serial.println("MDNS responder started");
    }

    server.on("/", []() {
      server.send(200, "text/html", indexPage);
    });

    server.on("/status", []() {
        server.send(200, "text/plain", makingCoffee ? "1" : "0");
    });

    server.on("/on", []() {
        makingCoffee = true;
        server.send(200, "text/plain", "ok");
    });

    server.on("/off", []() {
        makingCoffee = false;
        server.send(200, "text/plain", "ok");
    });

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    if (makingCoffee) {
      digitalWrite(led, LOW);
    } else {
      digitalWrite(led, HIGH);
    }
    server.handleClient();
    MDNS.update();
}
