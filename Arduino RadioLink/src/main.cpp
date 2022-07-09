/**
 * By Michael Wu on July 9, 2022.
 * reference from https://www.camelsoftware.com/2015/12/25/reading-pwm-signals-from-an-rc-receiver-with-arduino/
 *
 * Please wire up your PWM Inputs on Digital Pins and revise the *_SOURCE fields to the corresponding pin numbers.
 * For example, on Arduino Nano BLE 33
 * https://content.arduino.cc/assets/Pinout-NANOble_latest.png, the pins are D2, D3, D4, D5.
 *
 **/
#include <Arduino.h>
#define THROTTLE_SOURCE 2
#define STEERING_SOURCE 3
#define BRAKE_SOURCE 4
#define BUTTON_SOURCE 5

volatile unsigned long throttle_timer_start;
volatile unsigned long steering_timer_start;
volatile unsigned long brake_timer_start;
volatile unsigned long button_timer_start;

volatile int throttle_last_interrupt_time;
volatile int steering_last_interrupt_time;
volatile int brake_last_interrupt_time;
volatile int button_last_interrupt_time;

volatile int throttle_pulse_time;
volatile int steering_pulse_time;
volatile int brake_pulse_time;
volatile int button_pulse_time;

int output_throttle_pwm = 0;
int output_steering_pwm = 0;

void calcThrottleSignal()
{
  // record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  throttle_last_interrupt_time = micros();
  // if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(THROTTLE_SOURCE) == HIGH)
  {
    throttle_timer_start = micros();
  }
  // otherwise, the pin has gone LOW
  else
  {
    // only worry about this if the timer has actually started
    if (throttle_timer_start != 0)
    {
      // record the pulse time
      throttle_pulse_time = ((volatile int)micros() - throttle_timer_start);
      // restart the timer
      throttle_timer_start = 0;
    }
  }
}

void calcSteeringSignal()
{
  // record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  steering_last_interrupt_time = micros();
  // if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(STEERING_SOURCE) == HIGH)
  {
    steering_timer_start = micros();
  }
  // otherwise, the pin has gone LOW
  else
  {
    // only worry about this if the timer has actually started
    if (steering_timer_start != 0)
    {
      // record the pulse time
      steering_pulse_time = ((volatile int)micros() - steering_timer_start);
      // restart the timer
      steering_timer_start = 0;
    }
  }
}

void calcBrakeSignal()
{
  // record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  brake_last_interrupt_time = micros();
  // if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(BRAKE_SOURCE) == HIGH)
  {
    brake_timer_start = micros();
  }
  // otherwise, the pin has gone LOW
  else
  {
    // only worry about this if the timer has actually started
    if (brake_timer_start != 0)
    {
      // record the pulse time
      brake_pulse_time = ((volatile int)micros() - brake_timer_start);
      // restart the timer
      brake_timer_start = 0;
    }
  }
}

void calcButtonSignal()
{
  // record the interrupt time so that we can tell if the receiver has a signal from the transmitter
  button_last_interrupt_time = micros();
  // if the pin has gone HIGH, record the microseconds since the Arduino started up
  if (digitalRead(BUTTON_SOURCE) == HIGH)
  {
    button_timer_start = micros();
  }
  // otherwise, the pin has gone LOW
  else
  {
    // only worry about this if the timer has actually started
    if (button_timer_start != 0)
    {
      // record the pulse time
      button_pulse_time = ((volatile int)micros() - button_timer_start);
      // restart the timer
      button_timer_start = 0;
    }
  }
}

void printStatus()
{
  Serial.print("THROTTLE: ");
  Serial.print(throttle_pulse_time);

  Serial.print(" | STEERING: ");
  Serial.print(steering_pulse_time);

  Serial.print(" | BRAKE: ");
  Serial.print(brake_pulse_time);

  Serial.print(" | BUTTON: ");
  Serial.println(button_pulse_time);
}

void setup()
{
  throttle_timer_start = 0;
  steering_timer_start = 0;
  brake_timer_start = 0;
  button_timer_start = 0;
  attachInterrupt(THROTTLE_SOURCE, calcThrottleSignal, CHANGE);
  attachInterrupt(STEERING_SOURCE, calcSteeringSignal, CHANGE);
  attachInterrupt(BRAKE_SOURCE, calcBrakeSignal, CHANGE);
  attachInterrupt(BUTTON_SOURCE, calcButtonSignal, CHANGE);

  Serial.begin(115200);
}

void loop()
{
  printStatus();
  if (button_pulse_time > 1500)
  {
    // if button is pressed, use serial inputs
  }
  else
  {
    // if button is not pressed, serial input is ignored, use controller input
    output_throttle_pwm = throttle_pulse_time;
    output_steering_pwm = steering_pulse_time;
  }

  delay(20);
}
