#include <stdlib.h>
#include <Arduino.h>
#include "Platform.h"

// SETUP
#define EN        8  

//Direction pin
#define X_DIR     5
#define Y_DIR     6
#define Z_DIR     7

//Step pin
#define X_STP     2
#define Y_STP     3
#define Z_STP     4

//DRV8825
unsigned int delayTime=300000; //Delay between each pause (uS)

// Home Pins
#define X_HOME_PIN 9
#define Y_HOME_PIN 10

Platform* Platform::instance;

Platform::Platform(){
  x = -1;
  y = -1;
}

Platform* Platform::getInstance(){
    if(instance == 0){
        instance = new Platform();
        instance->homeBoth();
    }
    return instance;
}

//spins motors
void step(bool dir, int dirPin, int stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  delay(100);
  
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepperPin, HIGH);
    delayMicroseconds(delayTime); 
    digitalWrite(stepperPin, LOW);
    delayMicroseconds(delayTime); 
  }
}

void moveMoter(double distance, char axis){
  bool dir = false;
  
  if(distance < 0){
      dir = true;
  }
  if(axis == 'x'){
    step(!dir, X_DIR, X_STP, 50*abs(distance));
  }
  else if(axis == 'y'){
    step(dir, Y_DIR, Y_STP, 52.08*abs(distance));
  }
  else{
    Serial.print("ERROR: BAD AXIS INPUT");
  }
}

void Platform::moveTo(double xNew, double yNew){
  moveMoter(xNew-x, 'x');
  moveMoter(yNew-y, 'y');
  x = xNew;
  y = yNew;
}

void Platform::homeBoth(){
  int val = digitalRead(X_HOME_PIN);
  while(val == 0){
    val = digitalRead(X_HOME_PIN); 
    moveMoter(-.1, 'x');    
  }
  val = digitalRead(Y_HOME_PIN);
  while(val == 0){
    val = digitalRead(Y_HOME_PIN); 
    moveMoter(-.1, 'y');
  }
  
  moveMoter(73.5/2, 'x');
  x=73.5/2;
  y=0;
  
}

void Platform::setup(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);

  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);

  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);

  pinMode(EN, OUTPUT);

  digitalWrite(EN, LOW);  
  
}
