#include <Arduino.h>
#include <NewPing.h>
#include <PubSubClient.h>
#include <WiFi.h>

#define TAG "ULTRASONIC"

#define SONAR_NUM 3
#define MAX_DISTANCE 200

NewPing sonar[SONAR_NUM] = {
  NewPing(21, 22, MAX_DISTANCE),
  NewPing(19, 18, MAX_DISTANCE),
  NewPing(17, 16, MAX_DISTANCE)
};

const char* ssid = "TIM-20917905";
const char* password = "antonutti_lollipop5";
void connectToWifi()
{
  delay(10);
  ESP_LOGI(TAG, "Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  IPAddress address = WiFi.localIP();
  ESP_LOGI(TAG, "IP address: %d.%d.%d.%d", address[0], address[1], address[2], address[3]);
}

const char* mqttBroker = "192.168.1.133";
WiFiClient espClient;
PubSubClient mqttClient(espClient);
void setup() {
  // put your setup code here, to run once:
  connectToWifi();
  mqttClient.setServer(mqttBroker, 1883);
}

void reconnect()
{
  while (!mqttClient.connected())
  {
    if (mqttClient.connect("UltrasonicESP32Client"))
    {
      ESP_LOGI(TAG, "Connected");
    } else
    {
      ESP_LOGI(TAG, "Failed, rc=%d", mqttClient.state());
      delay(5000);
    }
  }
}

unsigned long now = 0;
unsigned long distance = 0;
long lastMsg = 0;
char msg[10];
char topic[20];
void loop() {
  // put your main code here, to run repeatedly:
  if (!mqttClient.connected())
  {
    reconnect();
  }
  
  now = millis();
  if (now - lastMsg > 300)
  {
    lastMsg = now;
    for (uint8_t i = 0; i < SONAR_NUM; i++)
    {
      delay(33);
      distance = sonar[i].ping_cm();
      distance = distance == 0 ? MAX_DISTANCE : distance;

      sprintf(topic, "ultrasonic/%u", i);   //Per iscriversi: topic "ultrasonic/+"
      sprintf(msg, "%lu", distance);
      mqttClient.publish(topic, msg);
      ESP_LOGI(TAG, "Published on topic: %s\tMessage:%s", topic, msg);
    }
  }
}