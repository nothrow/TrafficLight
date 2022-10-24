#include "TrafficLight.h"

#define REDLIGHT      0b0000100
#define YELLOWLIGHT   0b0000010
#define GREENLIGHT    0b0000001

#define DISABLED_0      0 // ...
#define REDSTABLE       1 // X..
#define REDTOGREEN_0    2 // X..
#define REDTOGREEN_1    3 // XX.
#define REDTOGREEN_2    4 // ..X
#define GREENSTABLE     5 // ..X
#define GREENTORED_0    6 // ..X
#define GREENTORED_1    7 // .X.
#define GREENTORED_2    8 // X..
#define DISABLED_1      9 // .X.

#define WILLBEGREEN(state) (state == GREENSTABLE || state == REDTOGREEN_0 || state == REDTOGREEN_1 || state == REDTOGREEN_2)
#define WILLBERED(state) (state == REDSTABLE || state == GREENTORED_0 || state == GREENTORED_1 || state == GREENTORED_2)
#define WILLBEDISABLED(state) (state == DISABLED_1 || state == DISABLED_0)

static int pinStates[] =
{
  0,
  REDLIGHT,
  REDLIGHT,
  REDLIGHT | YELLOWLIGHT,
  GREENLIGHT,
  GREENLIGHT,
  GREENLIGHT,
  YELLOWLIGHT,
  REDLIGHT,
  YELLOWLIGHT
};

TrafficLightStateMachine::TrafficLightStateMachine() : state_(0), interval_(800), lastchange_(0) {}

void TrafficLightStateMachine::setInterval(uint16_t interval_millis)
{
  interval_ = interval_millis;
}

void TrafficLightStateMachine::green() {
  if (!WILLBEGREEN(state_))
  {
    state_ = REDTOGREEN_0;
    lastchange_ = 0;
  }
}

void TrafficLightStateMachine::red()
{
  if (!WILLBERED(state_))
  {
    state_ = GREENTORED_0;
    lastchange_ = 0;
  }
}
void TrafficLightStateMachine::disable()
{
  if (!WILLBEDISABLED(state_))
  {
    state_ = DISABLED_0;
    lastchange_ = 0;
  }
}

static uint8_t stateMachine(uint8_t state)
{
  switch(state)
  {
    case DISABLED_0: return DISABLED_1;
    case REDSTABLE: return REDSTABLE;
    case REDTOGREEN_0: return REDTOGREEN_1;
    case REDTOGREEN_1: return REDTOGREEN_2;
    case REDTOGREEN_2: return GREENSTABLE;
    case GREENSTABLE: return GREENSTABLE;
    case GREENTORED_0: return GREENTORED_1;
    case GREENTORED_1: return GREENTORED_2;
    case GREENTORED_2: return REDSTABLE;
    case DISABLED_1: return DISABLED_0;
    default: return DISABLED_0; // fallback
  }
}

void TrafficLightStateMachine::update()
{
  auto now = getTime();
  if (now - lastchange_ > interval_)
  {
    state_ = stateMachine(state_);
    lastchange_ = now;
  }

  updatePins(pinStates[state_] & REDLIGHT, pinStates[state_] & YELLOWLIGHT, pinStates[state_] & GREENLIGHT);
}


// arduino specific bindings

#include <Arduino.h>

TrafficLight::TrafficLight() : TrafficLightStateMachine(), red_(0), yellow_(0), green_(0) { }

static void setPinState(int pin, bool state)
{
  if (state)
    digitalWrite(pin, 1);
  else
    digitalWrite(pin, 0);
}

void TrafficLight::attach(int rpin, int ypin, int gpin)
{
  red_ = rpin;
  yellow_ = ypin;
  green_ = gpin;

  pinMode(red_, OUTPUT);
  pinMode(yellow_, OUTPUT);
  pinMode(green_, OUTPUT);

  updatePins(false, false, false);
}

void TrafficLight::updatePins(bool red, bool yellow, bool green) const
{
  setPinState(red_, red);
  setPinState(yellow_, yellow);
  setPinState(green_, green);
}

unsigned long TrafficLight::getTime() const
{
  return millis();
}