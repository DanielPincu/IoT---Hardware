#include <WiFiNINA.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(3);
DallasTemperature sensors(&oneWire);

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  sensors.begin();

  WiFi.begin("S25","TryAgain");
}

void loop() {
  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);

  if (client.connect("iot-server-yp8e.onrender.com", 443)) {

    client.print("GET /payload?value=");
    client.print(t);
    client.println(" HTTP/1.1");
    client.println("Host: iot-server-yp8e.onrender.com");
    client.println();
    
  } 

  delay(1000);
}