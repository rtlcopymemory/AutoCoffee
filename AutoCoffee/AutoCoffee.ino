#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
#include <Wire.h>
#include "DS3231.h"

#include "credentials.h"
#include "index.h"

const char *ssid = SSID;
const char *password = PASSWORD;

RTClib RTC;
// DS3231 rtclock;

/*
    rtclock.setSecond(59);
    rtclock.setMinute(50);
    rtclock.setHour(14);
    rtclock.setDoW(4);
    rtclock.setDate(9);
    rtclock.setMonth(7);
    rtclock.setYear(20);
 */

ESP8266WebServer server(80);

const int led = 13;

bool makingCoffee = false;
uint32_t timerStop = 0;

// milliseconds/seconds
unsigned long precision = 3 * 100; // Calculated by taking 10 samples of actual time taken to switch off after the 0 seconds
// It's not accurate cause I'm not using a RTC but millis() instead. In my case it was 3 secs every 10 secs. seconds * 1000 => 3/10 * 1000 mills/secs => 300

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

void startTimer(int minutes, int seconds) {
  if (minutes < 0 || seconds < 0) return;
  timerStop = RTC.now().unixtime() + (minutes * 60) + seconds;
}

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    Serial.begin(115200);
    Wire.begin();
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
        // makingCoffee = true;
        startTimer(server.arg("minutes").toInt(), server.arg("seconds").toInt());
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

void updateStatus() {
  if (makingCoffee) {
      digitalWrite(led, LOW);
      Serial.print("Time: ");
      Serial.println(RTC.now().unixtime());
      Serial.print("Should stop at: ");
      Serial.println(timerStop);
      if (RTC.now().unixtime() >= timerStop) {
        makingCoffee = false;
        digitalWrite(led, HIGH);
      }
    } else {
      digitalWrite(led, HIGH);
    }
}

void loop(void)
{
    updateStatus();
    server.handleClient();
    MDNS.update();
    updateStatus();
}
