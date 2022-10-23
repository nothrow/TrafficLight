# TrafficLight

Library for implementing european-style of traffic light. It incorporates state machine that correctly transitions from green to red and back.

## Basic Use

### Instatiation

```c++
#include <TrafficLight.h>

/// creates new object
TrafficLight traffic();
```

### Setup

```c++
/// configures, which to which pins red/yellow/green lights/LEDs are connected
/// configures those pins as OUTPUT
traffic.attach(REDPIN, YELLOWPIN, GREENPIN);

/// low long does the transition take - how long does the "yellow" remains turned on
/// during transition
traffic.setInterval(1000);
```

### Loop

```c++
/// should be called as often as possible - is nonblocking, and updates internal state
traffic.update();

/// triggers state transition to red light
traffic.red();

/// triggers state transition to green light
traffic.green();

/// triggers state transition to blinking yellow light
traffic.disable();
```

See [examples/simple.cpp](examples/simple.cpp) for full example.
