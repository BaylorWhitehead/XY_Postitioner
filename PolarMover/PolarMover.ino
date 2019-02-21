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

// Physical Constants
// These values need to be set by calibration.
// Values are in centimeters
int xstepspercm = 55;
int ystepspercm = 55;
double xtracklength = 73.5; //This is the total length, from -x to +x.
double ytracklength = 54;

// Track position of carts.
double xPos = 0;
double yPos = 0;

// Signal IO
const int pwmPin = 0;    // Assign the pin we want to output a pulse on -- Alex's pulse wave output code

//DRV8825
int delayTime=300000; //Delay between each pause (uS)

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
//static void serialOut(Object[]);  <-- Would like to implement this function at some point to clean up code readability. Something like serialOut({"X Position: ", xPos, ", " , yPos}); ?



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
  //moveToBeginning('x');
  //moveToBeginning('y');
  
  // Move mic and speaker to center of track -- This is for calibrating step size.
  //centerCard('x');
  //centerCart('y');

  //Move mic and speaker to end (calibrate)
  moveToEnd('x');
  moveToEnd('y');

  // Sweep the arc of polar points for recording SPL
  // Assumes we are starting from ____? [TBD]
  //sweepPolarPoints();
  
  exit(0);
}

void step(boolean dir, byte dirPin, byte stepperPin, int steps)
{
  digitalWrite(dirPin, dir);
  

  delay(100);

  Serial.print("Step: ");
  for (int i = 0; i < steps; i++) {
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

  // Update position
  // First, figure out which cart we are moving, and whether the boolean value corresponds to positive or negative movement
  int dirPosNeg;
  if(dirPin == X_DIR) { // implies we are moving the x cart
    if(dir) dirPosNeg = 1;
    else dirPosNeg = -1;
  }
  else if(dirPin == Y_DIR) { // implies we are moving the y cart
    if(dir) dirPosNeg = -1;
    else dirPosNeg = 1;
  }
  // Then add/sub this movement amount from the Pos
  if(dirPin == X_DIR) xPos = xPos + (dirPosNeg * steps / xstepspercm);
  else if(dirPin == Y_DIR) yPos = yPos + (dirPosNeg * steps / ystepspercm);
  Serial.print("POS: (");
  Serial.print(xPos);
  Serial.print(",");
  Serial.print(yPos);
  Serial.println(")");
  Serial.println("-----");
  
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

static void moveToBeginning(char cart){
  // Choose which cart to control
  byte DIR, STP;
  double stepspercm;
  boolean movementDir;
  if(cart == 'x') {
    DIR = X_DIR;
    STP = X_STP;
    stepspercm = xstepspercm;
    movementDir = false;
  } else if(cart == 'y') {
    DIR = Y_DIR;
    STP = Y_STP;
    stepspercm = ystepspercm;
    movementDir = true;
  }
  /* 
  This needs to be updated; no longer an infinite loop. Instead, should query current location of the art, and issue movement commands until the position is at zero.
  Before this can be done, tracking must be implemented into the movement function (currently, step()).
  */
  // Reset the cart by continuously moving it backward
  while(1) {
      step(movementDir, DIR, STP, stepspercm);
  }
}

static void centerCart(char cart) { // Move the vertical-moving cart (speaker) to center
  // Choose which cart to control
  byte DIR, STP;
  double stepspercm;
  int tracklength;
  boolean movementDir;
  if(cart == 'x') {
    DIR = X_DIR;
    STP = X_STP;
    stepspercm = xstepspercm;
    tracklength = xtracklength;
    movementDir = true;
  } else if(cart == 'y') {
    DIR = Y_DIR;
    STP = Y_STP;
    stepspercm = ystepspercm;
    tracklength = ytracklength;
    movementDir = false;
  }
  
  // This assumes that the cart is at the 0cm position (edge of the track)
  for(int cms = 0; cms < tracklength/2; cms++) {
    step(movementDir, DIR, STP, stepspercm); // true = positive movement
    Serial.print("CMS: ");
    Serial.print(cms);
    Serial.print(" --> ");
  }
}

static void moveToEnd(char cart) {// cart --> choose which cart to control ('x' or 'y')
  // Assumes cart is at 0cm position (at edge of the track)
  byte DIR, STP;
  double stepspercm;
  int tracklength;
  boolean movementDir;
  if(cart == 'x') {
    DIR = X_DIR;
    STP = X_STP;
    stepspercm = xstepspercm;
    tracklength = xtracklength;
    movementDir = true;
  } else if(cart == 'y') {
    DIR = Y_DIR;
    STP = Y_STP;
    stepspercm = ystepspercm;
    tracklength = ytracklength;
    movementDir = false;
  }
  
  for(int cms = 0; cms < floor(tracklength); cms++) {
    step(movementDir, DIR, STP, stepspercm); // true = positive movement
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
  if( xPos < 0 || xPos > xtracklength || yPos > ytracklength )
    return false;
  else
    return true;
}

static double getXPos(double r, double thetaDeg) { 
  double thetaRad = thetaDeg*PI/180;
  return (xtracklength/2) + (r * cos(thetaRad)); // Shifts by xtracklength/2 because of translated origin
}
static double getYPos(double r, double thetaDeg) {
  double thetaRad = thetaDeg*PI/180;
  return r * sin(thetaRad);
}
static double getXDiff(double oldPos, double newPos) {
  return newPos - oldPos;
}
