#include "Platform.h"
#include "DigitalTrigger.h"

// Polar functions
static double getXPos(double r, double theta /*deg*/);
static double getYPos(double r, double theta /*deg*/);

void setup(){
  Platform *p = Platform::getInstance();
  DigitalTrigger *dt = DigitalTrigger::getInstance();
}

void loop(){
  delay(10000);
  
  Platform *p = Platform::getInstance();
  DigitalTrigger *dt = DigitalTrigger::getInstance();

  double xNew = 0;
  double yNew = 0;
  for(double i = 15; i<=45; i+=10){
    for(double w = 0; w<=180; w+=5 ){
      xNew = getXPos(i, w);
      yNew = getYPos(i, w);  
      p->moveTo(xNew, yNew);
      
      dt->pulse(100);
      delay(5000);
    }
  }

  exit(0);
}

static double getXPos(double r, double thetaDeg) { 
  double thetaRad = thetaDeg*PI/180;
  return (r * cos(thetaRad)); // Shifts by xtracklength/2 because of translated origin
}

static double getYPos(double r, double thetaDeg) {
  double thetaRad = thetaDeg*PI/180;
  return r * sin(thetaRad);
}
