// Author: Alex Goldstein
/* Description: Collection of functions for Metamaterials project. 
 *     Used to translate (r,theta) into cartesian coordinates (x,y). 
 *     Because the Arduino moves on a track, the origin on the polar plane is located at 1/2 * [width of the track] in cartesian space. (The track goes from 0 --> WIDTH.) 
 */

static bool isValidPos(double r, double theta /*deg*/);
static double getXPos(double r, double theta /*deg*/);
static double getYPos(double r, double theta /*deg*/);
static double getXDiff(double xOld, double xNew);
static double getYDiff(double yOld, double yNew);

// CONSTANTS
const double WIDTH = 1;
const double HEIGHT = 2;

void setup() {
  // Setup serial communication
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }
}

int numTimes = 0;
void loop() {
  if(numTimes == 0) {
    // TEST OF FUNCTIONS

    // State Width, Height consts
    Serial.print("WIDTH: ");
    Serial.println(WIDTH);
    Serial.print("HEIGHT: ");
    Serial.println(HEIGHT);

    double r = 1; // Test radius
    Serial.print("RADIUS: ");
    Serial.println(r);
    Serial.println();

    // Iterate through 0-->180 degrees
    // For each test, print: 
    //                    (r,theta): (x,y) --> [validity]
    for(int i = 0; i < 180; i++){
      Serial.print("IsValidPos(");
      Serial.print(r);
      Serial.print(",");
      Serial.print(i);
      Serial.print("): ");
       
      Serial.print("Cartesian(");
      Serial.print(getXPos(r,i));
      Serial.print(",");
      Serial.print(getYPos(r,i));
      Serial.print(") --> ");
      Serial.println(isValidPos(r,i)); //false
    }
    
    numTimes = 1;
  }
}


static bool isValidPos(double r, double thetaDeg) { // Checks if the given (r,thetaDeg) accessible on the track?
  double xPos = getXPos(r,thetaDeg);
  double yPos = getYPos(r,thetaDeg);
  if( xPos < 0 || xPos > WIDTH || yPos < 0 || yPos > HEIGHT )
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
