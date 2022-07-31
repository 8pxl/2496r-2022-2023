#include "main.h"
#include "global.hpp"
#include "util.hpp"


void spinTo(double target, double timeout, double tolerance)
{ 
  //resetting timers
  util::timer integralTimer;
  util::timer endTimer;
  util::timer timeoutTimer;

  //basic constants
  double kP = 2.1;
  double kI = 0;
  double kD = 0.1;
  double endTime = 1;

  //general vars
  double currHeading = glb::imu.get_heading();
  int dir = -util::dirToSpin(target,currHeading);
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
  while (!end)
  {
    currHeading = glb::imu.get_heading();
    
    //pee
    error = util::minError(target,(currHeading));

    //eye
    if (error > errorThreshold){
      integral += error;
    } 
    if (error <= tolerance && integralTimer.time() > integralTimeout){
      integral = 0;
    }
    else if (error >= tolerance ){
      integralTimer.start();
    }

    //dee
    derivative = error - prevError;
    
    //spin motors

    //double fwd is spin cw

    double rVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
    double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);

    chass.spinDiffy(rVel,lVel);
    

    if (error >= tolerance){
      endTimer.start();
    }

    if (endTimer.time() >= endTime){
      end = true;

    } 

      if (timeoutTimer.time() >= timeout){
        end = true;
      }

    prevError = error;
    pros::delay(15);
  }

  chass.stop("b");

} 