#include <Arduino.h>
#include <NewPing.h>
#include <PubSubClient.h>

#define SONAR_NUM 3
#define MAX_DISTANCE 200

NewPing sonar[SONAR_NUM] = {
  NewPing(21, 22, MAX_DISTANCE),
  NewPing(19, 18, MAX_DISTANCE),
  NewPing(17, 16, MAX_DISTANCE)
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (uint8_t i = 0; i < SONAR_NUM; i++)
  {
    delay(50);
    Serial.printf("%d = %d cm\n", i, sonar[i].ping_cm());
  }
  delay(1000);
}