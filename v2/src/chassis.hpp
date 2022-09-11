#ifndef __CHASSIS__
#define __CHASSIS__

#include "global.hpp"
#include "util.hpp"

namespace chas
{
  void spinTo(double target, double timeout, double tolerance);
  void drive(double target, double timeout, double tolerance);
  void odomDrive(double distance, double timeout, double tolerance);
  std::vector<double> moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias);
  void moveTo(util::coordinate target, double timeout, double lkp, double rkp, double rotationBias);
  void moveToPose(util::bezier curve, double timeout, double lkp, double rkp, double rotationBias);
}

void chas::spinTo(double target, double timeout, double tolerance)
{ 
  // timers
  util::timer endTimer;
  util::timer timeoutTimer;
  timeoutTimer.start();

  // basic constants
  double kP = 2.1;
  double kI = 0.1;
  double kD = 7;
  double endTime = 2000;

  // general vars
  double currHeading = robot::imu.degHeading();
  double prevHeading = currHeading;
  double error;
  double prevError;
  bool end = false;

  // eye vars
  double integral = 0;
  double integralThreshold = 10;

  // dee vars
  double derivative;
  
  // pid loop 
  while (!end)
  {

    currHeading = robot::imu.degHeading();
    int dir = -util::dirToSpin(target,currHeading);

    //pee
    error = util::minError(target,currHeading);
    glb::controller.print(0, 0, "%f", error);

    //eye
    integral = error <= tolerance ? 0 : fabs(error) < integralThreshold ? integral += error : integral;

    //dee
    derivative = error - prevError;
    prevError = error;

    //end conditions
    if (error >= tolerance)
    {
      endTimer.start();
    }

    end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

    // spin motors
    double rVel = dir * (error*kP + integral*kI + derivative*kD);
    double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
    robot::chass.spinDiffy(rVel,lVel);

    pros::delay(10);
  }
  robot::chass.stop("b");
} 

void chas::drive(double target, double timeout, double tolerance)
{ 
  // timers
  util::timer endTimer;
  util::timer timeoutTimer;
  timeoutTimer.start();

  // basic constants
  double kP = 0.1;
  double kI = 0;
  double kD = 0;
  double endTime = 100000;

  // general vars
  double error;
  double prevError;
  bool end = false;

  // eye vars
  double integral = 0;
  double integralThreshold = 10;

  // dee vars
  double derivative;
  
  // pid loop 
  robot::chass.reset();

  while (!end)
  {

    double currRotation = robot::chass.getRotation();

    //pee
    error = target - currRotation;
    glb::controller.print(0, 0, "%f", error);

    //eye
    integral = error <= tolerance ? 0 : fabs(error) < integralThreshold ? integral += error : integral;

    //dee
    derivative = error - prevError;
    prevError = error;

    //end conditions
    if (error >= tolerance)
    {
      endTimer.start();
    }

    end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

    // spin motors
    double rVel = (error*kP + integral*kI + derivative*kD);
    double lVel = (error*kP + integral*kI + derivative*kD);
    robot::chass.spinDiffy(rVel,lVel);

    pros::delay(10);
  }
  robot::chass.stop("b");
} 

void chas::odomDrive(double distance, double timeout, double tolerance)
{ 
  
  // resetting timers
  util::timer endTimer;
  util::timer timeoutTimer;
  timeoutTimer.start();

  // pid constants
  double kP = 2.1;
  double kI = 0;
  double kD = 0.1;
  double endTime = 1;

  // general vars
  double dist = -distance;
  double heading = robot::imu.radHeading();
  util::coordinate target(sin(2*PI-heading) * dist + glb::pos.x, cos(2*PI-heading) * dist + glb::pos.y);
  double prevRotation;
  double error;
  double prevError;
  bool end = false;

  // eye vars
  double integral = 0;
  double integralThreshold = 30;

  // dee vars
  double derivative;
  
  // pid loop 
  while (!end)
  {
    
    // pee
    error = dist - (dist - util::distToPoint(glb::pos,target));

    // eye
    integral = error <= tolerance ? 0 : fabs(error) < integralThreshold ? integral += error : integral;

    // dee
    derivative = error - prevError;
    prevError = error;

    // end conditions
    if (error >= tolerance)
    {
      endTimer.start();
    }

    end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

    // spin motors
    double vel = (error*kP + integral*kI + derivative*kD);
    robot::chass.spin(vel);

    pros::delay(10);
  }
  robot::chass.stop("b");
}  

std::vector<double> chas::moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias)
{
  double linearError = distToPoint(glb::pos,target);
  double linearVel = linearError*lkp;

  double currHeading =  robot::imu.degHeading(); //0-360
  double targetHeading = absoluteAngleToPoint(glb::pos, target); // -180-180
  // targetHeading = targetHeading >= 0 ? targetHeading + -180 : targetHeading - 180;
  targetHeading = targetHeading >= 0 ? targetHeading : 360 - fabs(targetHeading);  //conver to 0-360

  int dir = -util::dirToSpin(targetHeading,currHeading);

  double rotationError = util::minError(targetHeading,currHeading);
  double rotationVel = rotationError * rkp * dir; 

  // lowers overal speed in porportion to rotationError and rotationBias
  double lVel = (linearVel - (fabs(rotationVel) * rotationBias)) - rotationVel;
  double rVel = (linearVel - (fabs(rotationVel) * rotationBias)) + rotationVel;

  // glb::controller.print(0,0,"(%f, %f)\n", linearError,targetHeading);
  // spinTo(targetHeading, 1000000, 1);
  return std::vector<double> {lVel, rVel};
}

void chas::moveTo(util::coordinate target, double timeout, double lkp, double rkp, double rotationBias)
{
  // this one is pretty self explanatory
  util::timer timeoutTimer;
  timeoutTimer.start();

  while(timeoutTimer.time() < timeout)
  {
    std::vector<double> velocites = moveToVel(target, lkp, rkp, rotationBias); 
    robot::chass.spinDiffy(velocites[1],velocites[0]);
  }

  robot::chass.stop("b");
}


// void moveToPosePID(util::coordinate target, double finalHeading, double initialBias, double finalBias, double timeout, double initialHeading = robot::imu.degHeading())
void chas::moveToPose(util::bezier curve, double timeout, double lkp, double rkp, double rotationBias)
{
  
  // resolution in which to sample points along the curve
  int resolution = 100;

  // util::bezier curve = util::bezier(glb::pos,target,initialBias,finalBias, util::dtr(initialHeading),util::dtr(finalHeading));

  /* creates a look up table so values dont have to be calculated on the fly and dont need to be
  recalculated */
  std::vector<util::coordinate> lut = curve.createLUT(resolution);

  double t;
  double distTraveled = 0;
  double ratioTraveled;
  double curveLength = curve.approximateLength(lut, resolution);

  util::coordinate prevPos = glb::pos;
  util::coordinate targetPos;

  while(1)
  {
    /* approximates dist traveled along the curve by summing the distance between the current
    robot position and the previous robot position */
    distTraveled += util::distToPoint(prevPos, glb::pos);
    prevPos = glb::pos;

    // finds the closest calculated point in the look up table, (rounds up): 0.1 = 1)
    ratioTraveled = distTraveled/curveLength;
    t = std::ceil(ratioTraveled * resolution);

    // bc you count from zero
    targetPos = lut[t-1];

    std::vector<double> velocities = moveToVel(targetPos,0.1,0.1,0.1);
    robot::chass.spinDiffy(velocities[1], velocities[0]);

    // if t reaches the last point
    if (t == lut.size())
    {
      break;
    }
  }
  moveTo(lut[t], timeout, lkp, rkp, rotationBias);
}

#endif