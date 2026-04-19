#include <WiFiNINA.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ===== WIFI =====
const char* ssid = "S25";
const char* password = "TryAgain";

// ===== DS18B20 =====
#define VCC_PIN 2        // fake VCC 
#define ONE_WIRE_BUS 3   // DATA pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ===== HTTPS CLIENT =====
WiFiSSLClient client;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("BOOT");

  // power sensor 
  pinMode(VCC_PIN, OUTPUT);
  digitalWrite(VCC_PIN, HIGH);
  delay(1000);

  sensors.begin();

  // WiFi connect
  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi OK");
  Serial.println(WiFi.localIP());
}

void loop() {
  // read temperature
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  Serial.print("Temp: ");
  Serial.println(temp);

  // connect to Render (HTTPS)
  if (client.connect("iot-server-yp8e.onrender.com", 443)) {
    Serial.println("Connected to server");

    // send GET request
    client.print("GET /temp?value=");
    client.print(temp);
    client.println(" HTTP/1.1");
    client.println("Host: iot-server-yp8e.onrender.com");
    client.println("Connection: close");
    client.println();

  } else {
    Serial.println("Connection FAILED");
  }

  delay(5000);
}