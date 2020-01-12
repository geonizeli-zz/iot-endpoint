#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

#define SSID "clean link"

#define PASSWORD "a3r41959"

const String ORG = "hw8xak";
const String DEVICE_TYPE = "ESP32";
const String DEVICE_ID = "01";
#define DEVICE_TOKEN "wjeYlIdkz4w*HFD4WH"

const String CLIENT_ID = "d:"+ORG+":"+DEVICE_TYPE+":"+DEVICE_ID;
const String MQTT_SERVER = ORG + ".messaging.internetofthings.ibmcloud.com";


#define R1_TOPIC "iot-2/cmd/cmdR1/fmt/json"
#define R2_TOPIC "iot-2/cmd/cmdR2/fmt/json"
#define R3_TOPIC "iot-2/cmd/cmdR3/fmt/json"
#define R4_TOPIC "iot-2/cmd/cmdR4/fmt/json"
#define RGB_TOPIC "iot-2/cmd/cmdRGB/fmt/json"
#define BUZZER_TOPIC "iot-2/cmd/cmdBUZZER/fmt/json"

#define R1_PIN 14
#define R2_PIN 27
#define R3_PIN 26
#define R4_PIN 25

#define RED 39
#define GREEN 36
#define BLUE 15

#define BUZZER 4
#define CONNECT_PIN 2

WiFiClient wifiClient;

PubSubClient client(MQTT_SERVER.c_str(), 1883, wifiClient);

void setup() {
  Serial.begin(115200);

  pinMode(R1_PIN, OUTPUT);
  pinMode(R2_PIN, OUTPUT);
  pinMode(R3_PIN, OUTPUT);
  pinMode(R4_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(RED, INPUT);
  pinMode(GREEN, INPUT);
  pinMode(BLUE, INPUT);

  ledcAttachPin(RED, 0);
  ledcSetup(0, 1000, 10);
  ledcAttachPin(GREEN, 1);
  ledcSetup(1, 1000, 10);
  ledcAttachPin(BLUE, 2);
  ledcSetup(3, 1000, 10);

  digitalWrite(R1_PIN, LOW);
  digitalWrite(R2_PIN, LOW);
  digitalWrite(R3_PIN, LOW);
  digitalWrite(R4_PIN, LOW);
  digitalWrite(BUZZER, LOW);

  pinMode(CONNECT_PIN, OUTPUT);
  digitalWrite(CONNECT_PIN, LOW);

  WiFiManager wifiManager;

  wifiManager.setAPCallback(configModeCallback); 
  wifiManager.setSaveConfigCallback(saveConfigCallback); 

  wifiManager.autoConnect("ESP_AP");
  //wifiManager.startConfigPortal("ESP_AP");

  connectMQTTServer();
}

void loop() {
    client.loop();
}

void connectMQTTServer() {
  Serial.println("Connecting to MQTT Server...");
  if (client.connect(CLIENT_ID.c_str(), "use-token-auth", DEVICE_TOKEN)) {
    Serial.println("Connected to MQTT Broker");
    client.setCallback(callback);

    client.subscribe(R1_TOPIC);
    client.subscribe(R2_TOPIC);
    client.subscribe(R3_TOPIC);
    client.subscribe(R4_TOPIC);

    digitalWrite(CONNECT_PIN, HIGH);
  } else {
    Serial.print("error = ");
    Serial.println(client.state());
    connectMQTTServer();
  }
}

void callback(char* topic, unsigned char* payload, unsigned int length) {
  Serial.print("topic ");
  Serial.println(topic);
  StaticJsonBuffer<30> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(payload);

  if(!root.success())
  {
      Serial.println("Json Parse Error");
      return;
  }

  int value = root["value"];

  if (strcmp(topic, R1_TOPIC) == 1)
  {
    digitalWrite(R1_PIN, value);
  }
  else if (strcmp(topic, R2_TOPIC) == 1)
  {
    digitalWrite(R2_PIN, value);
  }
  else if (strcmp(topic, R3_TOPIC) == 1)
  {
    digitalWrite(R3_PIN, value);
  }
  else if (strcmp(topic, R4_TOPIC) == 1)
  {
    digitalWrite(R4_PIN, value);
  }
  else if (strcmp(topic, BUZZER_TOPIC) == 1)
  {
    digitalWrite(BUZZER, value);
  }
  else if (strcmp(topic, RGB_TOPIC) == 1)
  {
    
    if (value != 0)
    {
      int red = root["red"];
      int green = root["green"];
      int blue = root["blue"];
      
      ledcWrite(0, red * 1023 / 255);
      ledcWrite(1, green * 1023 / 255);
      ledcWrite(2, blue * 1023 / 255);
    }
    else {
      ledcWrite(0, 0);
      ledcWrite(1, 0);
      ledcWrite(2, 0);
    }
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {  
  Serial.println("AP mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void saveConfigCallback () {
  Serial.println("Saved settings");
  Serial.println(WiFi.softAPIP());
}
