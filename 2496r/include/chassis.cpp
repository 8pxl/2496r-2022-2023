#include "vex.h"

#include <cmath>


timer integralTimer;
timer endTimer;
timer timeoutTimer;

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
    double deltaRotation = std::abs(currHeading - prevHeading);
    
    if (deltaRotation > 300){
      deltaRotation = std::abs (mod(currHeading,360) - mod(prevHeading,360));
    }

    relativeHeading += (deltaRotation);

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

void moveDist(double target, double timeout, double tolerance){
  //resetting timers
  integralTimer.reset();
  endTimer.reset();
  timeoutTimer.reset();

  //resetting sensors
  frontLeft.resetRotation();
  backLeft.resetRotation();
  frontRight.resetRotation();
  backRight.resetRotation();

  //basic constants
  double kP = 2.1;
  double kI = 0;
  double kD = 0.1;
  double endTime = 1;

  //general vars
  double currRotation = 0;
  double error;
  double prevError;
  bool end = false;

  //eye vars
  double integral = 0;
  double integralTimeout = 0;
  double errorThreshold = 30;

  //dee vars
  double derivative;
  //pid loop 
  while (!end){
    currRotation = imu.heading();

    //pee
    error = target - currRotation;

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

    double rVel =  (error*kP + integral*kI + derivative*kD);
    double lVel =  -(error*kP + integral*kI + derivative*kD);

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