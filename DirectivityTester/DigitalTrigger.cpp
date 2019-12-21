#include <stdlib.h>
#include <Arduino.h>
#include "DigitalTrigger.h"

#define PWM_PIN 7    // Assign the pin we want to output a pulse on -- Alex's pulse wave output code

DigitalTrigger* DigitalTrigger::instance;

DigitalTrigger::DigitalTrigger(){

}

DigitalTrigger* DigitalTrigger::getInstance(){
    if(instance == 0){
        instance = new DigitalTrigger();
        instance->setup();
    }
    return instance;
}

void DigitalTrigger::setup(){
  pinMode(PWM_PIN, OUTPUT);
}

void DigitalTrigger::pulse(int time){
  int outValue = 255;
  digitalWrite(PWM_PIN, HIGH);

  delay(time); // Wait t/2 milliseconds (half the period)

  // After the time has passed, stop outputting 5V
  outValue = 0;
  digitalWrite(PWM_PIN, LOW);
}
