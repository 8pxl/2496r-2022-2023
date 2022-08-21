#include "global.hpp"
#include "util.hpp"
#include <cmath>
#include <vector>

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
    double tx = sin(2*PI-glb::imu.get_heading()) * dist + glb::pos.x;
    double ty = cos(2*PI-glb::imu.get_heading()) * dist + glb::pos.y;
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
      error = dist - (dist-util::distToPoint(util::coordinate (glb::x, glb::y),util::coordinate(tx,ty)));

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

  std::vector<double> moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias)
  {
    double linearError = distToPoint(glb::pos,target);
    double linearVel = linearError*lkp;

    double currHeading =  glb::imu.get_heading() > 0 ? util::rtd(glb::imu.get_heading()) : util::rtd(glb::imu.get_heading()) + 360; //0-360
    double targetHeading = 180-absoluteAngleToPoint(target); // -180-180

    targetHeading = targetHeading >= 0 ? targetHeading : fabs(targetHeading) + 180;

    int dir = util::dirToSpin(targetHeading,currHeading);

    double rotationError = util::minError(targetHeading,currHeading);
    double rotationVel = rotationError * rkp * dir; 

    double lVel = (linearVel - fabs(rotationVel) * rotationBias) - rotationVel;
    double rVel = (linearVel - fabs(rotationVel) * rotationBias) + rotationVel;

    return std::vector<double> {lVel, rVel};
  }

  void moveTo(util::coordinate target, double timeout, double lkp, double rkp, double rotationBias)
  {
    util::timer timeoutTimer;
    timeoutTimer.start();

    while(timeoutTimer.time() < timeout)
    {
      std::vector<double> velocites = moveToVel(target, lkp, rkp, rotationBias); 
      robot::chass.spinDiffy(velocites[1],velocites[0]);
    }

    robot::chass.stop("b");
  }

// void moveToPosePID(util::coordinate target, double finalHeading, double initialBias, double finalBias, double timeout, double initialHeading = glb::imu.get_heading())
  void moveToPose(util::bezier curve, double timeOut, double lkp, double rkp, double rotationBias)
  {
    
    int resolution = 100;

    // util::bezier curve = util::bezier(glb::pos,target,initialBias,finalBias, util::dtr(initialHeading),util::dtr(finalHeading));

    std::vector<util::coordinate> lut = curve.createLUT(resolution);

    double t;
    double distTraveled = 0;
    double ratioTraveled;
    double curveLength = curve.approximateLength(lut, resolution);

    util::coordinate prevPos = glb::pos;
    util::coordinate targetPos;

    while(1)
    {
      distTraveled += util::distToPoint(prevPos, glb::pos);
      prevPos = glb::pos;

      ratioTraveled = distTraveled/curveLength;
      t = std::ceil(ratioTraveled * resolution);

      targetPos = lut[t-1];

      std::vector<double> velocities = moveToVel(targetPos,0.1,0.1,0.1);
      robot::chass.spinDiffy(velocities[1], velocities[0]);

      if (t == lut.size())
      {
        break;
      }
    }

    moveTo(lut[t], 10000, lkp, rkp, rotationBias);
  }

}