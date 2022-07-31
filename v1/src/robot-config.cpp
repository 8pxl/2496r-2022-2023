#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
motor leftFly = motor(PORT16, ratio6_1, false);
motor rightFly = motor(PORT15, ratio6_1, false);
controller Controller1 = controller(primary);
motor frontLeft = motor(PORT3, ratio18_1, false);
motor frontRight = motor(PORT9, ratio18_1, false);
motor backLeft = motor(PORT5, ratio18_1, false);
motor backRight = motor(PORT10, ratio18_1, false);
encoder vertEncoder = encoder(Brain.ThreeWirePort.A);
inertial imu = inertial(PORT20);
encoder horizEncoder = encoder(Brain.ThreeWirePort.C);
motor rightIntake = motor(PORT4, ratio18_1, false);
motor leftIntake = motor(PORT11, ratio18_1, false);
digital_out indexer = digital_out(Brain.ThreeWirePort.E);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}