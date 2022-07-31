#include <cmath>
#include "common.h"

#include "vex.h"
#include "includes.cpp"
#include "odom.cpp"
#include "chassis.cpp"
#include "purePursuit.cpp"
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
  
  coordinate p1[] = {coordinate(179,460), coordinate(243,456), coordinate(286,456), coordinate(335,457), coordinate(407,455), coordinate(434,424)};
  std::vector<coordinate> one(p1, p1 + sizeof(p1)/ sizeof(coordinate));
  moveToPurePursuit(one, 20, 20, 20);

  // imu.calibrate();
  // wait(3,sec);
  // vex::thread t(odom);
  // spinTo(90,7000,1);
  // spinTo(180,5000,1);
  // spinTo(270,5000,1);
  // spinTo(0,5000,1);
  // int rpmTarget = 550;

  while (1) {
    //  printf("%f", absoluteAngleToPoint(0,0));
    // printf("%f\n", imu.heading());
    // printf("%f\n",horizEncoder.rotation(deg));
    if (!pressed){

      // if (Controller1.ButtonUp.pressing()){
      //   targetUp();
      //   pressed = true;
      // }

      // else if (Controller1.ButtonDown.pressing()){
      //   targetDown();
      //   pressed = true;
      // }

      if (Controller1.ButtonR2.pressing()){

        // spinTo(absoluteAngleToPoint(0,0), 50000, 1);
        printf("%f", absoluteAngleToPoint(0,0));
        // spinTo(90,5000,5);
        pressed = true;
        
      }
      if(Controller1.ButtonL2.pressing()){
        indexer.set(true);
        wait(40,msec);
        indexer.set(false);
        wait(40,msec);
        indexer.set(true);
        wait(40,msec);
        indexer.set(false);
        wait(40,msec);
        indexer.set(true);
        wait(40,msec);
        indexer.set(false);
        pressed = true;
        
      }
    }
  
    if (!Controller1.ButtonDown.pressing() && ! Controller1.ButtonUp.pressing() && ! Controller1.ButtonR2.pressing()){
      pressed = false;
    }

    if(!Controller1.ButtonR2.pressing()){
      frontLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      frontRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      backLeft.spin(directionType::rev,(Controller1.Axis3.value() + Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      backRight.spin(directionType::fwd,(Controller1.Axis3.value() - Controller1.Axis1.value()) *(0.094),voltageUnits::volt);
      pressed = false;
    }

    if(Controller1.ButtonR1.pressing()){
      rightIntake.spin(reverse,100,pct);
      leftIntake.spin(fwd,100,pct);
    }
    else{
      rightIntake.stop(coast);
      leftIntake.stop(coast);
    }
  

    
    // leftFly.spin(reverse,600 , velocityUnits::rpm);
    // rightFly.spin(reverse,600, velocityUnits::rpm);

    // printf("%f\n", leftFly.velocity(rpm));
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
