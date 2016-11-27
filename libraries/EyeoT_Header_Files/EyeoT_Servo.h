// constants related to the servo motor
#include <Debug.h>
#include <Servo.h>            // for light switch servo motor

// Servo angles
#define ANGLE_ON 105    // predefined servo negative angle 
#define ANGLE_OFF 70  // predefined servo positive angle               
#define THRESHOLD_ON 525
#define THRESHOLD_OFF 420
#define TOLERANCE_FEEDBACK 40

// Calibration values
#define minDegrees 30
#define maxDegrees 150
#define minFeedback 188
#define maxFeedback 438
#define tolerance 5 // max feedback measurement error

// Control and feedback pins
#define SERVO_PIN 9
#define FEEDBACK_PIN A0

//light switch positions
enum SWITCH {
  UNDEF,
  OFF,
  ON
};

SWITCH lightSwitch = UNDEF; // initialize undefined
Servo lightSwitchServo; // create servo object

// define on, off, tolerance
SWITCH getState()
{
  int FEEDBACK = analogRead(FEEDBACK_PIN);
  DEBUG_PRINTDEC(FEEDBACK);
  if ( ((FEEDBACK) > (THRESHOLD_ON - TOLERANCE_FEEDBACK)) && ((FEEDBACK) < (THRESHOLD_ON + TOLERANCE_FEEDBACK)) ){
    DEBUG_PRINTLN("Switch On State");
    
    return ON;
  }
  else if ( ((FEEDBACK) > (THRESHOLD_OFF - TOLERANCE_FEEDBACK)) && (FEEDBACK) < ((THRESHOLD_OFF + TOLERANCE_FEEDBACK)) ){
    DEBUG_PRINTLN("Switch Off State");
    return OFF;
  }
  else {
    DEBUG_PRINTLN("Undefined Switch State");
    return UNDEF;
  }
}

SWITCH Move(SWITCH state)
{
  // Start the move...
  lightSwitchServo.attach(SERVO_PIN);
  delay(200);
  if(state == ON){
    lightSwitchServo.write(ANGLE_ON); // 75 // 50
  }
  else if(state == OFF){
    lightSwitchServo.write(ANGLE_OFF); // 105 // 120
  }
    while(getState() != state){}
    delay(100);
    lightSwitchServo.detach();
    delay(200);

  return getState();
}


