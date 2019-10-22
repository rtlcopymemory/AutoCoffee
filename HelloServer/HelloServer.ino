#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "index.h"
#include "wifiCredentials.h"

ESP8266WebServer server(80);

const int relay = 0;

String redirectString(String IP) {
  String redirectionHTML = {
    "<head>"
    "<meta http-equiv=\"refresh\" content=\"0; URL=http://"
    };

  String tmp2 = redirectionHTML + IP;

  String tmp3 = {
    "/\" />"
    "</head>"
  };
  return tmp2 + tmp3;
}

void handleRoot() {
  digitalWrite(relay, 1);
  server.send(200, "text/html", indexHTML);
  digitalWrite(relay, 0);
}

void handleAction() {
  if (server.method() == HTTP_POST) {
    server.send(404, "text/html", redirectString(WiFi.localIP().toString()));
    digitalWrite(relay, HIGH);
    Serial.println("Making coffee...\n");
    delay(5000);
    digitalWrite(relay, LOW);
    Serial.println("Done...\n");
  }
}

void handleNotFound() {
  digitalWrite(relay, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(relay, 0);
}

void setup(void) {
  // relay
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);

  // Server setup
  Serial.begin(115200);
  Serial.println("Automatic Coffee Machine\n");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("CONNECTING...\n");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/action", handleAction);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
