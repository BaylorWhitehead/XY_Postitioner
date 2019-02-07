// SETUP

#define EN        8  

//Direction pin
#define X_DIR     6
#define Y_DIR     5
#define Z_DIR     7

//Step pin
#define X_STP     3
#define Y_STP     2
#define Z_STP     4 


const int pwmPin = 0;    // Assign the pin we want to output on - alex code

const double WIDTH = 1;
const double HEIGHT = 2;

//DRV8825
int delayTime=300000; //Delay between each pause (uS)
int stps=254;// Steps to move 200 might be 360 degrees

//22.7272 steps per cm


static bool isValidPos(double r, double theta /*deg*/);
static double getXPos(double r, double theta /*deg*/);
static double getYPos(double r, double theta /*deg*/);
static double getXDiff(double xOld, double xNew);
static double getYDiff(double yOld, double yNew);


//3687 steps = .73m --> 50.50 steps / cm
double stepspercm = 50.50;
int tracklength = 73; // 88cm total, from midpoint-midpoint at ends is 73cm. 
void step(boolean dir, byte dirPin, byte stepperPin, int steps)

{

  digitalWrite(dirPin, dir);

  delay(100);

  for (int i = 0; i < steps; i++) {
    Serial.print("Step: ");
    Serial.print(i);
    Serial.print(" / " );
    Serial.println(steps);
    
    digitalWrite(stepperPin, HIGH);

    delayMicroseconds(delayTime); 

    digitalWrite(stepperPin, LOW);

    delayMicroseconds(delayTime); 

  }

}

void setup(){
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
  pinMode(X_DIR, OUTPUT); pinMode(X_STP, OUTPUT);

  pinMode(Y_DIR, OUTPUT); pinMode(Y_STP, OUTPUT);

  pinMode(Z_DIR, OUTPUT); pinMode(Z_STP, OUTPUT);

  pinMode(EN, OUTPUT);

  digitalWrite(EN, LOW);

  pinMode(pwmPin, OUTPUT); // This needs to be set in your program at some point before you call pulse().

}
//delay(5000);
int stepsmade = 0;
void loop(){
//  step(true, Y_DIR, Y_STP, 254);
//  delay(100000);
//  step(false, Y_DIR, Y_STP, 254);
  //step(false, Y_DIR, Y_STP, 1000); //Y, Clockwise
  Serial.print("HI");
  for(int cms = 0; cms < tracklength/2; cms++) {
    step(true, Y_DIR, Y_STP, stepspercm); //Y, Clockwise
    Serial.print(cms);
  }
  double x = 0;
  double y = 0;


  /*for(double i =0.05; i<=0.45; i+=0.1){
    for(double w = 0; w<=180; w+=5 ){
      Serial.print("Input: ");
      Serial.print(i);
      Serial.print(", ");
      Serial.print(w);
      Serial.print(" --> ");
      if(isValidPos(i, w)){
        double xNew = getXPos(i, w);
        double yNew = getYPos(i, w);

        
        double xMov = getXDiff(x, xNew);
        double yMov = getYDiff(y, yNew);    

        Serial.print("(");
        Serial.print(xNew*100);
        Serial.print(",");
        Serial.print(yNew*100);
        Serial.print(")");

        Serial.print("(");
        Serial.print(xMov*100);
        Serial.print(",");
        Serial.print(yMov*100);
        Serial.print(")");

        bool ifXMoveNegative = true;
        bool ifYMoveNegative = true;
        if(xMov > 0) ifXMoveNegative = false;
        if(yMov > 0) ifYMoveNegative = false;
        step(!ifXMoveNegative, X_DIR, X_STP, 2272.72*abs(xMov)); //X, Clockwise
        step(!ifYMoveNegative, Y_DIR, Y_STP, 2272.72*abs(yMov)); //Y, Clockwise
        delay(150*max(xMov,yMov)*100);
        x = xNew;
        y = yNew;
        pulse(pwmPin, 100); //450ms pulse
      }
      Serial.println("");
    }
  }*/
  exit(0);
}

void pulse(int pwmPin, int t){
  int outValue = 255;
  digitalWrite(pwmPin, HIGH);

  delay(t); // Wait t/2 milliseconds (half the period)

  // After the time has passed, stop outputting 5V
  outValue = 0;
  digitalWrite(pwmPin, LOW);
}

static bool isValidPos(double r, double thetaDeg) { // Checks if the given (r,thetaDeg) accessible on the track?
  double xPos = getXPos(r,thetaDeg);
  double yPos = getYPos(r,thetaDeg);
  if( xPos < 0 || xPos > WIDTH || yPos > HEIGHT )
    return false;
  else
    return true;
}

static double getXPos(double r, double thetaDeg) { 
  double thetaRad = thetaDeg*PI/180;
  return (WIDTH/2) + (r * cos(thetaRad)); // Shifts by WIDTH/2 because of translated origin
}
static double getYPos(double r, double thetaDeg) {
  double thetaRad = thetaDeg*PI/180;
  return r * sin(thetaRad);
}
static double getXDiff(double xOld, double xNew) {
  return xNew - xOld;
}
static double getYDiff(double yOld, double yNew) {
  return yNew - yOld;
}
