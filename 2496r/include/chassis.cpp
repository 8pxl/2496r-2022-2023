#include "vex.h"

#include <cmath>


timer integralTimer;
timer endTimer;
timer timeoutTimer;

void moveDist(){
    frontLeft.spin(fwd,100,rpm);
  backLeft.spin(fwd,100,rpm);
  frontRight.spin(fwd,100,rpm);
  backRight.spin(fwd,100,rpm);
  wait(3,sec);
  printf("%s\n" , "2");
}





int dirToSpin(double target, double unscaled){
  // -1 for clockwise
  // 1 for counterclockwise
  double currHeading = unscaled - 180;
  if (currHeading > 0){
    if (currHeading - 180 <= target){
      return 1;
    }
    return -1;
  }

  else if (currHeading < 0){
    if(currHeading + 180 >= target){
      return -1;
    }
    return 1;
  }
  return -1;
}

void spinTo(double target, double timeout, double tolerance){
  //resetting timers
  integralTimer.reset();
  endTimer.reset();
  timeoutTimer.reset();

  //basic constants
  double kP = 2.1;
  double kI = 0;
  double kD = 0.1;
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
    printf("%i\n", dir);
    //relative heading
    // printf("%f\n" , error);
    currHeading = imu.heading();
    double deltaRotation = (currHeading - prevHeading);
    
    if (std::abs(deltaRotation) > 300){
      deltaRotation = mod(currHeading,360) - mod(prevHeading,360);
    }

    relativeHeading += deltaRotation * dir;

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

    double rVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
    double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);

    frontLeft.spin(fwd,lVel,rpm);
    backLeft.spin(fwd,lVel,rpm);
    frontRight.spin(fwd,rVel,rpm);
    backRight.spin(fwd,rVel,rpm);
    
    //end condition
    // printf("%lu\n", endTimer.time()/1000);
    if (error >= tolerance){
      endTimer.reset();
    }

    if (endTimer.time() >= endTime){
      end = true;

    } 

    if (timeoutTimer.time() >= timeout){
      end = true;
    }

    prevError = error;
    wait(15,msec);
  }
  frontLeft.stop(brakeType::hold);
  backLeft.stop(brakeType::hold);
  frontRight.stop(brakeType::hold);
  backRight.stop(brakeType::hold);

}