#include <WiFi.h>

char ssid[] = "";
char password[] = "";

#define BUILT_IN_LED 2


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(BUILT_IN_LED, OUTPUT);

  connect();

  Serial.println("Connected to the WiFi network");
  digitalWrite(BUILT_IN_LED, HIGH);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connect();
  }
}

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
}