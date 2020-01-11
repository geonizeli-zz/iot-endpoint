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

#define RGB_TOPIC "iot-2/cmd/rgb/fmt/json"
#define BZR_TOPIC "iot-2/cmd/buzzer/fmt/json"
#define SKT1_TOPIC "iot-2/cmd/socket_1/fmt/json"
#define SKT2_TOPIC "iot-2/cmd/socket_2/fmt/json"
#define SKT3_TOPIC "iot-2/cmd/socket_3/fmt/json"
#define SKT4_TOPIC "iot-2/cmd/socket_4/fmt/json"

#define RGBR_PIN 13
#define RGBG_PIN 14
#define RGBB_PIN 15
#define BRZ_PIN 4
#define SKT1_PIN 14
#define SKT2_PIN 27
#define SKT3_PIN 26
#define SKT4_PIN 25

#define CONNECT_PIN 2

WiFiClient wifiClient;

PubSubClient client(MQTT_SERVER.c_str(), 1883, wifiClient);

void setup() {
    Serial.begin(115200);

    pinMode(RGBR_PIN, OUTPUT);
    digitalWrite(RGBR_PIN, LOW);

    pinMode(RGBG_PIN, OUTPUT);
    digitalWrite(RGBG_PIN, LOW);

    pinMode(RGBB_PIN, OUTPUT);
    digitalWrite(RGBB_PIN, LOW);

    pinMode(BRZ_PIN , OUTPUT);
    digitalWrite(BRZ_PIN , LOW);

    pinMode(SKT1_PIN, OUTPUT);
    digitalWrite(SKT1_PIN, LOW);

    pinMode(SKT2_PIN, OUTPUT);
    digitalWrite(SKT2_PIN, LOW);

    pinMode(SKT3_PIN, OUTPUT);
    digitalWrite(SKT3_PIN, LOW);

    pinMode(SKT4_PIN, OUTPUT);
    digitalWrite(SKT4_PIN, LOW);

    pinMode(CONNECT_PIN, OUTPUT);
    digitalWrite(CONNECT_PIN, LOW);

    WiFiManager wifiManager;

    wifiManager.setAPCallback(configModeCallback); 
    wifiManager.setSaveConfigCallback(saveConfigCallback); 

    wifiManager.autoConnect("ESP_AP");

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

    client.subscribe(RGB_TOPIC);
    client.subscribe(BZR_TOPIC);
    client.subscribe(SKT1_TOPIC);
    client.subscribe(SKT2_TOPIC);
    client.subscribe(SKT3_TOPIC);
    client.subscribe(SKT4_TOPIC);

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

    if (strcmp(topic, BZR_TOPIC) == 1)
    {
      digitalWrite(BRZ_PIN, value);
    }
    else if (strcmp(topic, SKT1_TOPIC) == 1)
    {
      digitalWrite(SKT1_PIN, value);
    }
    else if (strcmp(topic, SKT2_TOPIC) == 1)
    {
      digitalWrite(SKT2_PIN, value);
    }
    else if (strcmp(topic, SKT3_TOPIC) == 1)
    {
      digitalWrite(SKT3_PIN, value);
    }
    else if (strcmp(topic, SKT4_TOPIC) == 1)
    {
      digitalWrite(SKT4_PIN, value);
    }
    else // So is the RGB_TOPIC
    {
      // TODO:
      // build a function to control pwm from rgb pins
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
