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

// Physical Constants
//3687 steps = .73m --> 50.50 steps / cm
double xstepspercm = 50.50;
double ystepspercm = 50.50;
int xtracklength = 73; // 88cm total, from midpoint-midpoint at ends is 73cm. 
int ytracklength = 73; 

// Signal IO
const int pwmPin = 0;    // Assign the pin we want to output a pulse on -- Alex's pulse wave output code

// Physical track 
const double WIDTH = 1;
const double HEIGHT = 2;

//DRV8825
int delayTime=300000; //Delay between each pause (uS)
int stps=254;// Steps to move 200 might be 360 degrees

// ABSTRACT FUNCTION DECLARATIONS
// Polar Coordinates
static bool isValidPos(double r, double theta /*deg*/);
static double getXPos(double r, double theta /*deg*/);
static double getYPos(double r, double theta /*deg*/);
static double getXDiff(double xOld, double xNew);
static double getYDiff(double yOld, double yNew);
// Cart Movement
static void resetCart(char cart);
static void centerCart(char cart);
static void sweepPolarPoints();
// Signal IO
static void pulse(int pwmPin, int t);



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

void loop(){
  // Cart must start from 0cm position
  //resetCart('x');
  //resetCart('y');
  
  // Move mic and speaker to center of track -- This is for calibrating step size.
  //centerCard('x');
  //centerCart('y');

  // Sweep the arc of polar points for recording SPL
  // Assumes we are starting from ____? [TBD]
  sweepPolarPoints();
  
  exit(0);
}

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);

  delay(100);

  for (int i = 0; i < steps; i++) {
    Serial.print("Step: ");
    Serial.print(i);
    Serial.print(" / " );
    Serial.print(steps);
    Serial.print(" ... ");
    
    digitalWrite(stepperPin, HIGH);

    //delayMicroseconds(delayTime); 

    digitalWrite(stepperPin, LOW);

    //delayMicroseconds(delayTime); 

  }
  Serial.println("");
}

static void sweepPolarPoints() {
  double x = 0;
  double y = 0;
  for(double i =0.05; i<=0.45; i+=0.1){
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
  }
}

static void resetCart(char cart){
  // Choose which cart to control
  byte DIR, STP;
  double stepspercm;
  if(cart == 'x') {
    DIR = X_DIR;
    STP = X_STP;
    stepspercm = xstepspercm;
  } else if(cart == 'y') {
    DIR = Y_DIR;
    STP = Y_STP;
    stepspercm = ystepspercm;
  }
  // Reset the cart by continuously moving it backward
  while(1) {
      step(false, DIR, STP, stepspercm);
  }
}
static void centerCart(char cart) { // Move the vertical-moving cart (speaker) to center
  // Choose which cart to control
  byte DIR, STP;
  double stepspercm;
  if(cart == 'x') {
    DIR = X_DIR;
    STP = X_STP;
    stepspercm = xstepspercm;
  } else if(cart == 'y') {
    DIR = Y_DIR;
    STP = Y_STP;
    stepspercm = ystepspercm;
  }
  
  // This assumes that the cart is at the 0cm position (edge of the track)
  for(int cms = 0; cms < xtracklength/2; cms++) {
    step(true, DIR, STP, stepspercm); // true = positive movement
    Serial.print(cms);
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
