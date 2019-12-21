#include "Platform.h"

// Signal IO
const int pwmPin = 7;    // Assign the pin we want to output a pulse on -- Alex's pulse wave output code

// ABSTRACT FUNCTION DECLARATIONS
static bool isValidPos(double r, double theta /*deg*/);
static double getXPos(double r, double theta /*deg*/);
static double getYPos(double r, double theta /*deg*/);

// Physical Constants - cm
#define X_Track_Length 73.5 //This is the total length, from -x to +x.
#define Y_Track_Length 54

// Signal IO
static void pulse(int pwmPin, int t);

void setup(){
  Platform *p = Platform::getInstance();
  p->setup();
  
  pinMode(pwmPin, OUTPUT); // This needs to be set in your program at some point before you call pulse().
}

void loop(){
  delay(10000);
  sweepPolarPoints();
  exit(0);
}

static void sweepPolarPoints() {
  Platform *p = Platform::getInstance();

  double xNew = 0;
  double yNew = 0;
  for(double i = 15; i<=45; i+=10
  ){
    for(double w = 0; w<=180; w+=5 ){
      xNew = getXPos(i, w);
      yNew = getYPos(i, w);
      if(isValidPos(xNew, yNew)){   
        p->moveTo(xNew, yNew);
        pulse(7, 100);
        delay(5000);
      }
    }
  }
}

static void pulse(int pwmPin, int t){
  int outValue = 255;
  digitalWrite(pwmPin, HIGH);

  delay(t); // Wait t/2 milliseconds (half the period)

  // After the time has passed, stop outputting 5V
  outValue = 0;
  digitalWrite(pwmPin, LOW);
}

static bool isValidPos(double xPos, double yPos) { // Checks if the given (r,thetaDeg) accessible on the track?
  if(abs(xPos) < X_Track_Length/2 || yPos > Y_Track_Length ){
    Serial.print("bad pos");
    return false;
  }
  else{  
    Serial.print("GUCCI"); 
    return true;
  }
}

static double getXPos(double r, double thetaDeg) { 
  double thetaRad = thetaDeg*PI/180;
  return (r * cos(thetaRad)); // Shifts by xtracklength/2 because of translated origin
}
static double getYPos(double r, double thetaDeg) {
  double thetaRad = thetaDeg*PI/180;
  return r * sin(thetaRad);
}
