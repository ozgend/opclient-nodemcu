#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#include "page_index.h"

#define SERIAL_PORT 115200

String _ssid = "OPHOST";
String _password = "ophost@lok!";

// String _ssid = "Cyberdyne";
// String _password = "de@dline201!x";

// relay pin to control - wire relay for COM+NO
int _relayPin = 13;
int _pinValue = LOW;

ESP8266WebServer server(80);
HTTPClient http;

int writePin(int value)
{
  _pinValue = value;
  digitalWrite(_relayPin, value);
  return _pinValue;
}

void responseIndex()
{
  server.send(200, "text/html", page_INDEX);
}

void responseApi(String value)
{
  server.send(200, "application/json", value);
}

void handleOn()
{
  writePin(LOW);
  responseIndex();
}

void handleOff()
{
  writePin(HIGH);
  responseIndex();
}

void handleSwitch()
{
  // /switch?to=1|0
  int to = server.arg("to").toInt();
  int pinValue = to == 0 ? HIGH : LOW;
  pinValue = writePin(pinValue);
  String switchResponse = "{ \"pinValue\": $PIN_VALUE$ }";
  switchResponse.replace("$PIN_VALUE$", String(pinValue));
  responseApi(switchResponse);
}

void setup(void)
{
  Serial.begin(SERIAL_PORT);
  delay(10);

  // set relay pin
  pinMode(_relayPin, OUTPUT);
  writePin(HIGH);

  // init wifi
  Serial.println("--init-wifi-");
  WiFi.begin(_ssid, _password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print("..");
  }

  Serial.println("ssid: " + _ssid);

  // http handlers
  server.on("/", responseIndex);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/switch", handleSwitch);

  // http server start
  server.begin();
  Serial.println("--server-start--");

  Serial.print("host: http://");
  Serial.println(WiFi.localIP());
}

void loop(void)
{
  server.handleClient();
}
