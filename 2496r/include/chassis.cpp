#include "vex.h"

#include <cmath>
timer integralTimer;
timer endTimer;
timer timeoutTimer;

int dirToSpin(double target, double unscaled){
  // 0 for clockwise
  // 1 for counterclockwise
  double currHeading = unscaled - 180;
  if (currHeading > 0){
    if (currHeading - 180 <= target){
      return 1;
    }
    return 0;
  }

  else if (currHeading < 0){
    if(currHeading + 180 >= target){
      return 0;
    }
    return 1;
  }
  return 0;
}

void spinTo(double target, double timeout, double tolerance){
  //resetting timers
  integralTimer.reset();
  endTimer.reset();
  timeoutTimer.reset();

  //basic constants
  double kP = 0.7;
  double kI = 0;
  double kD = 0 ;
  double endTime = 1;

  //general vars
  double currHeading = imu.heading();
  int dir = dirToSpin(target,currHeading);
  double prevHeading = currHeading;
  double error;
  double prevError;
  double relativeHeading = 0;
  double relativeDist = std::abs ( target - currHeading);
  bool end = false;

  //eye vars
  double integral = 0;
  double integralTimeout = 0;
  double errorThreshold = 30;

  //dee vars
  double derivative;

  //pid loop
  while (!end){
    //relative heading
    currHeading = imu.heading();
    relativeHeading += std::abs(currHeading - prevHeading);
    prevHeading = currHeading;
    
    //pee
    error = relativeDist - relativeHeading;

    //eye
    if (error > errorThreshold){
      integral += error;
    } 
    if (error <= tolerance && integralTimer.value() > integralTimeout){
      integral = 0;
    }
    else if (error >= tolerance ){
      integralTimer.reset();
    }

    //dee
    derivative = error - prevError;
    
    //spin motors

    //double fwd is spin cw

    double rVel = pow(-1,dir) * (error*kP + integral*kI + derivative*kD);
    double lVel = pow(-1,dir) * (error*kP + integral*kI + derivative*kD);

    frontLeft.spin(fwd,lVel,rpm);
    backLeft.spin(fwd,lVel,rpm);
    frontRight.spin(fwd,rVel,rpm);
    backRight.spin(fwd,rVel,rpm);
    
    //end condition

    if (error >= tolerance){
      endTimer.reset();
    }

    if (endTimer.time() >= endTime){
      end = true;
    } 

    if (timeoutTimer >= timeout){
      end = true;
    }

    prevError = error;
    wait(15,msec);
  }

}