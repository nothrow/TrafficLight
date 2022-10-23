#include <Arduino.h>
#include <TrafficLight.h>

TrafficLight trafficLight;

#define RED_PIN 4
#define YELLOW_PIN 5
#define GREEN_PIN 6

#define UPDATEINTERVAL 5000

unsigned long lastUpdate = 0;
int state = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting up...");

  trafficLight.attach(RED_PIN, YELLOW_PIN, GREEN_PIN);
  trafficLight.setInterval(1200);
}

void loop()
{
  trafficLight.update();
// periodically switch between red, green, red, green, disabled (blinking)
  if ((lastUpdate + UPDATEINTERVAL) < millis())
  {
    lastUpdate = millis();

    state++;
    switch(state%5)
    {
      case 0: case 2:
        Serial.println("Switching to red");
        trafficLight.red();
        break;
      case 1: case 3:
        Serial.println("Switching to green");
        trafficLight.green();
        break;
      
      default:
        Serial.println("Switching to disabled");
        trafficLight.disable();
        break;
    }
  }
}
