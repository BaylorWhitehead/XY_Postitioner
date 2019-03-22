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
double xPos = xtracklength/2;
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
static double getDiff(double xOld, double xNew);
// Cart Movement
//static void resetCart(char cart);
//static void centerCart(char cart);
//static void sweepPolarPoints();
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
  
  //Move to start Positions
  delay(10);
  //Loop through Polar Pos
  sweepPolarPoints();
  //moveMoter(5, 'x');
  exit(0);
}

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

static void sweepPolarPoints() {
  double x = 0;
  double y = 0;
  double xNew = 0;
  double yNew = 0;
  double xMov = 0;
  double yMov = 0;
  
  for(double i = 15; i<=45; i+=10
  ){
    for(double w = 0; w<=180; w+=5 ){
      
      Serial.print("Input: ");
      Serial.print(i);
      Serial.print(", ");
      Serial.print(w);
      Serial.print(" --> ");
      
      xNew = getXPos(i, w);
      yNew = getYPos(i, w);
      if(isValidPos(xNew, yNew)){
        xMov = getDiff(x, xNew);
        yMov = getDiff(y, yNew);    
        x = xNew;
        y = yNew;
        
        Serial.print("(");
        Serial.print(xNew);
        Serial.print(",");
        Serial.print(yNew);
        Serial.print(")");

        Serial.print("(");
        Serial.print(xMov);
        Serial.print(",");
        Serial.print(yMov);
        Serial.print(")");
        
        moveMoter(xMov, 'x');
        moveMoter(yMov, 'y');
        delay(3000);
      }
      Serial.println("");
    }
  }
}

static void moveMoter(double distance, char axis){
  boolean dir = false;
  
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

static void pulse(int pwmPin, int t){
  int outValue = 255;
  digitalWrite(pwmPin, HIGH);

  delay(t); // Wait t/2 milliseconds (half the period)

  // After the time has passed, stop outputting 5V
  outValue = 0;
  digitalWrite(pwmPin, LOW);
}

static bool isValidPos(double xPos, double yPos) { // Checks if the given (r,thetaDeg) accessible on the track?
  if( xPos < 0 || xPos > xtracklength || yPos > ytracklength ){
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
  return (xtracklength/2) + (r * cos(thetaRad)); // Shifts by xtracklength/2 because of translated origin
}
static double getYPos(double r, double thetaDeg) {
  double thetaRad = thetaDeg*PI/180;
  return r * sin(thetaRad);
}
static double getDiff(double oldPos, double newPos) {
  return newPos - oldPos;
}
