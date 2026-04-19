#include <WiFiNINA.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "S25";
const char* password = "TryAgain";

OneWire oneWire(3);
DallasTemperature sensors(&oneWire);

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Booting...");

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  sensors.begin();

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.println("Connected!");
}

void loop() {
  sensors.requestTemperatures();
  int t = sensors.getTempCByIndex(0);

  Serial.print("Reading temperature... ");
  Serial.println(t);

  if (client.connect("iot-server-yp8e.onrender.com", 443)) {

    client.print("GET /temp?value=");
    client.print(t);
    client.println(" HTTP/1.1");
    client.println("Host: iot-server-yp8e.onrender.com");
    client.println();

    Serial.println("Temperature sent");

  } else {
    Serial.println("Server FAIL");
  }

  delay(1000);
}