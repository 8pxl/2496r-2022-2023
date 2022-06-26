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

#include "vex.h"
#include <cmath>
#include "includes.cpp"
#include "odom.cpp"
#include "chassis.cpp"

using namespace vex;
// A global instance of competition
competition Competition;

void pre_auton(void) {
  vexcodeInit();
}

void autonomous(void) {
}

void usercontrol(void) {
  bool pressed = true;
  imu.calibrate();
  wait(3,sec);
  vex::thread t(odom);
  // spinTo(90,7000,1);
  printf("%f", absoluteAngleToPoint(0,0));
  // spinTo(180,5000,1);
  // spinTo(270,5000,1);
  // spinTo(0,5000,1);
  int rpmTarget = 550;

  while (1) {
    // printf("%f\n", imu.heading());
    // printf("%f\n",horizEncoder.rotation(deg));
    if (!pressed){

      if (Controller1.ButtonUp.pressing()){
        targetUp();
        pressed = true;
      }

      else if (Controller1.ButtonDown.pressing()){
        targetDown();
        pressed = true;
      }

      if (Controller1.ButtonR2.pressing()){

        spinTo(absoluteAngleToPoint(0,0), 50000, 0);
        // spinTo(190,5000,5);
        pressed = true;
        
      }
    }
  
    if (!Controller1.ButtonDown.pressing() && ! Controller1.ButtonUp.pressing()){
      pressed = false;
    }

    if(!Controller1.ButtonR2.pressing()){
      frontLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      frontRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      backLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      backRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      pressed = false;
    }
  

    
    leftFly.spin(forward,averageRpm()+ fPid(550), velocityUnits::rpm);
    rightFly.spin(reverse, averageRpm() + fPid(550), velocityUnits::rpm);
    // printf("%i\n", averageRpm());
    // printf("%i\n", rpmTarget);
    // printf("%s\n",' ');
    wait(20, msec);
  }
}

int main() {
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  pre_auton();

  while (true) {
    wait(100, msec);
  }
}
