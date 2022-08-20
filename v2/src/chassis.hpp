#include "global.hpp"
#include "util.hpp"

namespace chassis
{
  void spinTo(double target, double timeout, double tolerance)
  { 
    //timers
    util::timer endTimer;
    util::timer timeoutTimer;
    timeoutTimer.start();

    //basic constants
    double kP = 1.8;
    double kI = 0;
    double kD = 0.3;
    double endTime = 1;

    //general vars
    double currHeading = glb::imu.get_heading();
    double prevHeading = currHeading;
    double error;
    double prevError;
    bool end = false;

    //eye vars
    double integral = 0;
    double integralTimeout = 0;
    double integralThreshold = 10;

    //dee vars
    double derivative;
    
    //pid loop 
    while (!end)
    {

      currHeading = glb::imu.get_heading();
      int dir = -util::dirToSpin(target,currHeading);

      //pee
      error = util::minError(target,(currHeading));
      glb::controller.print(0, 0, "%f", error);

      //eye
      integral = error <= tolerance ? 0 : fabs(error) < integralThreshold ? integral += error : integral;

      //dee
      derivative = error - prevError;
      prevError = error;

      //end conditions
      if (error <= tolerance)
      {
        endTimer.start();
      }

      end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

      //spin motors
      double rVel = dir * (error*kP + integral*kI + derivative*kD);
      double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
      robot::chass.spinDiffy(rVel,lVel);

      pros::delay(10);
    }
    robot::chass.stop("b");
  } 

  void drive(double distance, double timeout, double tolerance)
  { 
    
    //resetting timers
    util::timer endTimer;
    util::timer timeoutTimer;
    timeoutTimer.start();

    //pid constants
    double kP = 2.1;
    double kI = 0;
    double kD = 0.1;
    double endTime = 1;

    //general vars
    double dist = -distance;
    double tx = sin(2*PI-glb::imu.get_heading()) * dist + glb::x;
    double ty = cos(2*PI-glb::imu.get_heading()) * dist + glb::y;
    double prevRotation;
    double error;
    double prevError;
    bool end = false;

    //eye vars
    double integral = 0;
    double integralThreshold = 30;

    //dee vars
    double derivative;
    
    //pid loop 
    while (!end)
    {
      
      //pee
      error = dist - (dist-util::distToPoint(glb::x,glb::y,tx,ty));

      //eye
      integral = error <= tolerance ? 0 : fabs(error) < integralThreshold ? integral += error : integral;

      //dee
      derivative = error - prevError;
      prevError = error;

      //end conditions
      if (error <= tolerance)
      {
        endTimer.start();
      }

      end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

      //spin motors
      double vel = (error*kP + integral*kI + derivative*kD);
      robot::chass.spin(vel);

      pros::delay(10);
    }
    robot::chass.stop("b");
  }  
}