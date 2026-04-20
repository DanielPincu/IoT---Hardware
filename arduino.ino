#include <WiFiNINA.h>
#include <WebSocketsClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(3);
DallasTemperature sensors(&oneWire);
WebSocketsClient ws;

void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  sensors.begin();

  WiFi.begin("S25","TryAgain");

  ws.beginSSL("iot-server-yp8e.onrender.com", 443, "/");
}

void loop() {
  ws.loop();

  if (!ws.isConnected()) return;

  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);

  String msg = String(t);
  ws.sendTXT(msg);

  delay(1000);
}