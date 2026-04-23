#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT11

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

unsigned long channelID = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_API_KEY";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("DHT ERROR");
    delay(2000);
    return;
  }

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int status = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (status == 200) {
    Serial.println("Data sent ✔");
  } else {
    Serial.print("Error: ");
    Serial.println(status);
  }

  delay(20000);
}
