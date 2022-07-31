using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor leftFly;
extern motor rightFly;
extern controller Controller1;
extern motor frontLeft;
extern motor frontRight;
extern motor backLeft;
extern motor backRight;
extern encoder vertEncoder;
extern inertial imu;
extern encoder horizEncoder;
extern motor rightIntake;
extern motor leftIntake;
extern digital_out indexer;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );