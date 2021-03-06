#include <Debug.h>        // contains print functions for verbose debugging.
#include <EyeoT_BLE_Commands_and_Controls.h> // contains all EyeoT BLE commands and their actions
#include <EyeoT_Servo.h>  // contains all EyeoT servo constants and control functions
#include <EyeoT_Light_Strip.h> // contains functions and constants related to color and pin mapping of the indicator light strip    
// #include <CurieTimerOne.h>

//callback function used for timer delay
/*void checkPhyiscalSwitchState(){
  currentState.setValue(getState());
}
*/

void setup() {


  //Servo adjustments
 //pinMode(9, INPUT);
  
  Serial.begin(9600);

  //Timer functions for delays

  //Attach the interupt function to the callback function checkPhyiscalSwitchState()
  //CurieTimerOne.attachInterrupt(checkPhyiscalSwitchState);

  // set the local name peripheral advertises
  blePeripheral.setLocalName("EyeoT_BLE");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(eyeoTService.uuid());

  // add service and characteristics
  blePeripheral.addAttribute(eyeoTService);
  blePeripheral.addAttribute(currentCommand);
  blePeripheral.addAttribute(currentResponse);
  blePeripheral.addAttribute(currentState);
  
  currentCommand.setValue(0); // indicator lights off
  currentResponse.setValue(6); // not ready
  currentState.setValue(0); //undefined state

  // advertise the service
  blePeripheral.begin();
  DEBUG_PRINTLN(("Bluetooth device active, waiting for connections..."));

  // initialize indicator light strip
  strip.begin(); // Initialize pins for output
  strip.updateLength(TOTAL_NUMPIXELS);
  for (int i=0; i < TOTAL_NUMPIXELS; i++)
    strip.setPixelColor(i, off);
  strip.setBrightness(255);
  strip.show();// Update strip (light it up)  

  // get current switch state (update from undefined)
  lightSwitch = getState();
  currentState.setValue(getState()); //needs testing
  currentResponse.setValue(READY);

  //pinMode(9,OUTPUT);

  // start timer interrupt; every 1 second it reads the current state of the device
  // CurieTimerOne.start(1000000, checkPhyiscalSwitchState);
}

/*
 * This function accepts a BLE_COMMAND and tries to execute the command, returning its result.
 *
 * Input: A single BLE_COMMAND.
 *
 * Output: A boolean commandSuccessful representing whether or not the command was properly executed.
*/

bool serviceCommmand(unsigned short command)
{           
   switch (command)
   {
      currentResponse.setValue(NOT_READY); // for all cases, indicate that the Arduino is no longer ready to recieve further commands
      case NO_LIGHT: // turn off indicator lights
          DEBUG_PRINT("turn off indicator lights ");
          return setLightColor(off);
      case RED_LIGHT: // turn indicator lights to red
          DEBUG_PRINT("set indicator lights to red ");
          return setLightColor(red);
      case GREEN_LIGHT: // turn indicator lights to green 
          DEBUG_PRINT("set indicator lights to green ");
          return setLightColor(green);
      case BLUE_LIGHT: // turn indicator lights to blue
          DEBUG_PRINT("set indicator lights to blue ");
          return setLightColor(blue);
      case SERVO_ON: // move servo motor so light switch turns on
          if(!setLightColor(off)){
            DEBUG_PRINTLN("ERROR: Indicator lights not properly turned off!");
          }
          delay(250); // save power for the servo motor
          lightSwitch = Move(ON);
          currentState.setValue(DEVICE_ON);
          DEBUG_PRINT("turn light switch on ");
            if (lightSwitch == ON)
          {
            setLightColor(green);
            delay(2000);
            setLightColor(off);
            return true;
          }
          return false;
      case SERVO_OFF: // move servo motor so light switch turns off
         if(!setLightColor(off)){
            DEBUG_PRINTLN("ERROR: Indicator lights not properly turned off!");
         }
         delay(250);
         lightSwitch = Move(OFF);
         currentState.setValue(DEVICE_OFF);
         DEBUG_PRINT("turn light switch off "); 
         if (lightSwitch == OFF)
          {
            setLightColor(red);
            delay(2000);
            setLightColor(off);
            return true;
          }
          return false;
      default: 
         DEBUG_PRINTLN("ERROR: Unknown BLE Command Received!");
         return false;
   }
}

void loop(){

// need to add a hardware interrupt that fires only when the switch is different
// listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  // currentState.setValue(getState());
  if (central) {
    //now that a device is connected, pause the timer for state reads
   // CurieTimerOne.pause();
    
    DEBUG_PRINT("Connected to central: ");
    // print the central's MAC address:
    DEBUG_PRINTLN(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      
      // if the remote device writes a command, use it's value
      if (currentCommand.written()) {
        currentResponse.setValue(COMMAND_ACK); // indicate that the command has been received
        DEBUG_PRINT("BLE Command Received: ");
        DEBUG_PRINTDEC(currentCommand.value());
        DEBUG_PRINTLN(" ");
        DEBUG_PRINT("Command ");       
        if(serviceCommmand(currentCommand.value() - 48)){
          currentResponse.setValue(COMMAND_SUCCESS); // indicate that the command has been received
         //currentState.setValue(getState());
          DEBUG_PRINTLN("executed successfully!");  
        }
        else{
          currentResponse.setValue(COMMAND_FAILURE); // indicate that the command failed
         // currentState.setValue(getState());
          DEBUG_PRINTLN("ERROR: Command failed!");     
        }
        currentResponse.setValue(READY); // indicate that the Arduino is free to accept further commands
       // currentState.setValue(getState());
        DEBUG_PRINTLN("Arduino ready for future command(s)!");       
      }
    }

    // when the central disconnects, print it out:
    DEBUG_PRINT(F("Disconnected from central: "));
    //CurieTimerOne.resume(); //resume the timer once device is disconnected
    DEBUG_PRINTLN(central.address());
  }

}
