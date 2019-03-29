#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "page_index.h"
#include "json_templates.h"

#define SERIAL_PORT 115200

String _ssid = "SSID";
String _password = "PASSWORD";

// relay pin to control - wire relay for COM+NO
int _relayPin = 13;
int _pinValue = LOW;

ESP8266WebServer server(80);

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
  // host/switch?to=1|0
  int to = server.arg("to").toInt();
  int pinValue = to == 0 ? LOW : HIGH;
  pinValue = writePin(pinValue);
  responseApi(json_response_switch.replace(var_pin_value, pinValue));
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