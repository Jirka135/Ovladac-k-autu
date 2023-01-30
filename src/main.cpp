#include <WiFi.h>
#include <M5StickCPlus.h>

const char* ssid = "jirka";
const char* password = "jirka123";

const char* serverAddress = "10.0.0.50"; // Replace with the IP address of the server ESP32
const int serverPort = 80;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;
  if (!client.connect(serverAddress, serverPort)) {
    Serial.println("Could not connect to server");
    delay(1000);
    return;
  }

  String message = "cs";
  client.print(message);
  Serial.println("Message sent to server");

  String response = client.readStringUntil('\r');
  Serial.println("Response from server: " + response);

  client.stop();
  delay(3000);
}