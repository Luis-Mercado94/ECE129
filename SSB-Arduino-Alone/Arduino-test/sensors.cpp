#include "sensors.h"
const int AMT_MEAS = 10;
const int HEIGHT = 10;


bool checkProximitySensor()
{
  return digitalRead(PROXIMITY_PIN);
}

bool checkBin()
{
  unsigned long duration;
  unsigned int average, sum, distance;
  distance = average = sum = 0;
  for (int i = 0; i < AMT_MEAS; i++)
  {
    digitalWrite(PING_TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(PING_TRIG_PIN, HIGH);
    delayMicroseconds(10); //send 10 uS pulse
    digitalWrite(PING_TRIG_PIN, LOW);
    duration = pulseIn(PING_ECHO_PIN, HIGH);
    distance = duration * 0.034 / 2;
//    Serial.println(distance);
    sum += distance;
  }
  average = sum / AMT_MEAS;
  return ((average <= HEIGHT) ? FULL : NOT_FULL);
}

int runCV()
{
  delay (1000);
  return digitalRead(CV_INPUT_PIN0);
}

void printState(SSB ssb) {

}
