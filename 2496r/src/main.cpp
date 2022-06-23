/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// leftFly              motor         11              
// rightFly             motor         7               
// Controller1          controller                    
// frontLeft            motor         3               
// frontRight           motor         9               
// backLeft             motor         5               
// backRight            motor         10              
// vertEncoder          encoder       A, B            
// imu                  inertial      20              
// horizEncoder         encoder       C, D            
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

#include "includes.cpp"
#include "odom.cpp"
#include "chassis.cpp"

using namespace vex;
// using namespace std;
// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  bool run = true;

  // vex::thread t(odom);
  // odom();
  vertEncoder.setPosition(0,deg);
  horizEncoder.setPosition(0,deg);
  imu.calibrate();
  while (1) {
    // spinTo(35,5,5);
    // printf("%f\n", imu.heading());
    // printf("%f\n",horizEncoder.rotation(deg));
    if (run){
      if (Controller1.ButtonUp.pressing()){
        targetUp();
        run = false;
    }

      else if (Controller1.ButtonDown.pressing()){
        targetDown();
        run = false;
    }
    }

    if (!Controller1.ButtonDown.pressing() && ! Controller1.ButtonUp.pressing()){
      run = true;
    }

    if (Controller1.ButtonR2.pressing() && run){

      // spinTo(absoluteAngleToPoint(0,0), 5, 5);
      spinTo(35,5,5);
      run = false;
      
    }
    if (!Controller1.ButtonDown.pressing() && ! Controller1.ButtonUp.pressing()){
      run = true;
    }
  
    frontLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
    frontRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
    backLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
    backRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
    
    leftFly.spin(forward,averageRpm()+ fPid(rpmTarget), velocityUnits::rpm);
    rightFly.spin(reverse, averageRpm() + fPid(rpmTarget), velocityUnits::rpm);

    // printf("%i\n", averageRpm());
    // printf("%i\n", rpmTarget);
    // printf("%s\n",' ');
    wait(20, msec);
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
