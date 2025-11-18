# Gauge stepper motor driver for VID6608 and clones

This library implements driver for Arduino framework
for following driver chips for analog gauges with microstepping support:

* VID6606 (2 motors)
* VID6608 (4 motors)
* VT6608
* AX1201728SG
* BY8920
* many others

Driver chips with microstepping is the recommended way to drive such motors,
they provide much more relailabe and smooth movement with reduced noise and
to avoid skipping steps.

This library is developed by inspiration from SwitecX12 library, many thanks to author.

This library has following features:

* More precise Datasheet complaince
* Another smoothing method (requires less calculations)
* Optimized homing
* Extended API's




## Setting zero

Motor is set to zero by moving whole scale and kept bouncing on the one of
dead positions. This library provides optimized way to perform homing: it does
1/2 of scale forward, then full scale backward. This helps to reduce bouncing
like in the classical "full scale back" method.

## Basic example

Simple code to activate the library.

```cpp
#include <Arduino.h>
#include <vid6608.h>

// standard X25.168 range 315 degrees at 1/3 degree steps
#define STEPS (320 * 12)

#define PIN_STEP 26
#define PIN_DIR 27
#define PIN_RESET 2

vid6608 motor1(PIN_STEP, PIN_DIR, STEPS);

unsigned long nextMoveTime = 0;
uint16_t nextMovePos = 0;

void setup(void)
{
  // Run the motor against the stops
  motor1.zero();
  // Plan next move (in loop())
  motor1.moveTo(100);
}

void loop(void)
{
  // the motor only moves when you call update
  motor1.loop();
}
```



```
export PLATFORMIO_CI_SRC=examples/RandomMove/RandomMove.cpp
platformio ci --board uno -l src/
```
