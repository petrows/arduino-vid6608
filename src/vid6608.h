/*
 * VID6608 gauge motor driver library
 * by Petrows, 2025
 *
 * https://github.com/petrows/arduino-vid6608
 *
 * Licensed under the GPL, see LICENSE.txt for details.
 */

#ifndef vid6608_h
#define vid6608_h

#include <Arduino.h>

// Default max steps for VID6608 motor + x27.168 clones
#define VID6608_MAX_STEPS 320 * 12
#define VID6608_DEFAULT_ZERO_SPEED 600 // in microseconds

class vid6608 {
  public:
    // Initialize the motor driver
    vid6608(int stepPin, int dirPin, uint16_t maxSteps = VID6608_MAX_STEPS);
    // Resets zero position to actual 0 position
    // Uses optimized method to reduce bouncing end-stops
    void zero(uint16_t delay = VID6608_DEFAULT_ZERO_SPEED);
    // Move to absolute position
    // Warning: this function is asynchronous
    // Actual movement is done in the loop() function
    void moveTo(uint16_t position);
    // Returns false if we are in move
    bool isMoving();
    // Returns true is stopped
    bool isStopped() { return !isMoving(); }
    // Returns current absolute position
    uint16_t getPosition();
    // Main loop function, must be called often to perform the movement
    void loop();
    // Interface to manage acceleration table
    // Inspired by: https://github.com/clearwater/SwitecX25/pull/31
    typedef struct {
      uint16_t distance;            // in steps
      uint16_t delay;               // in microseconds
    } AccelTable;
    template <typename T, size_t N> void setAccelTable(T (&table)[N]) {
      accelTable = table;
      accelTableSize = N;
      accelMaxDistance = accelTable[accelTableSize - 1].distance;
      accelMaxDelay = accelTable[accelTableSize - 1].delay;
    };
  private:
    enum MoveState {
      STATE_IDLE = 0,
      STATE_MOVING = 1,
    };
    enum MoveDirection {
      MOVE_NONE = -1,
      MOVE_FORWARD = 0,
      MOVE_BACKWARD = 1,
    };
    uint16_t maxSteps;              // Maximum steps for the motor
    uint16_t currentPosition;       // Current position in steps
    uint16_t targetPosition;        // Target position in steps
    uint16_t targetPositionNext;    // Target position in steps (scheduled for next move)
    uint16_t moveLeft;              // How long we have to move left
    uint16_t moveDone;              // How long we have traveled
    int stepPin;                    // Pin connected to step input
    int dirPin;                     // Pin connected to direction input
    MoveState moveState;            // Current move state
    MoveDirection moveDirection;    // Current move state
    MoveDirection dirPinState;      // Direction pin last state, tri-state to force update on first step
    AccelTable *accelTable;         // Accel table can be modified
    uint16_t accelTableSize;        // How many rows in the acceleration table
    uint16_t accelMaxDistance;      // Max distance from the acceleration table
    uint16_t accelMaxDelay;         // Max delay from the acceleration table
    // Actual move
    void step(MoveDirection direction, uint16_t delay);
    // Get current acceleration speed (delay)
    uint16_t getDelay(uint16_t distance);
};

#endif // vid6608_h
