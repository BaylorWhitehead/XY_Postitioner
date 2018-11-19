//Please note some of the base code for input pins and rotation was found on the internet for a starting point.

#define EN        8  

//Direction pin
#define X_DIR     5 
#define Y_DIR     6
#define Z_DIR     7

//Step pin
#define X_STP     2
#define Y_STP     3 
#define Z_STP     4 

//Square Wave output pin
const int pwmPin = 0;    // Assign the pin we want to output on - alex code



//DRV8825
int delayTime=300000; //Delay between each pause (uS)
int stps=254;// Steps to move 200 might be 360 degrees

//spins motors
void step(boolean dir, byte dirPin, byte stepperPin, int steps)
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

//sets up all the pins
void setup(){

  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);

  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);

  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);

  pinMode(EN, OUTPUT);

  digitalWrite(EN, LOW);

  pinMode(pwmPin, OUTPUT); 

}

//the loop moves the positioner in its 16 by 16 grid and sends a square wave to signal the mirophone to trigger
void loop(){
  delay(5000);
  for(int i =0; i<16; i++){
    if(i>0){
      step(false, X_DIR, X_STP, stps); //X, Clockwise
    }
    for(int y=0; y<16; y++){
      if(y>0){
          step(false, Y_DIR, Y_STP, stps); //Y, Clockwise
        }  
      delay(1500);
      pulse(pwmPin, 100); //450ms pulse
      delay(5500);
    }
    delay(100);
    step (true, Y_DIR, Y_STP,stps*8);
    delay(100);
  
  }
  exit(0);

}

//creates square wave
void pulse(int pwmPin, int t){
  int outValue = 255;
  digitalWrite(pwmPin, HIGH);

  delay(t); // Wait t/2 milliseconds (half the period)

  // After the time has passed, stop outputting 5V
  outValue = 0;
  digitalWrite(pwmPin, LOW);
}
