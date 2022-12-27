#ifndef __CHASSIS__
#define __CHASSIS__

#include "groups.hpp"
#include "sensors.hpp"
#include "util/util.hpp"

namespace lib
{
    class chassis
    {
        private:
            lib::diffy chass;
            lib::imu imu;
            util::coordinate pos;
            double DL;
            double DR;

        public:
            chassis(lib::diffy mtrs, lib::imu inertial, util::coordinate position, double dl = 0, double dr = 0) : chass(mtrs), imu(inertial), pos(position), DL(dl), DR(dr) {}

            void updatePos(double rx, double ry);
            void spinTo(double target, double timeout, util::pidConstants constants);
            void aspin(double target, double timeout, util::pidConstants constants);
            void drive(double target, double timeout, util::pidConstants constants);
            void autoDrive(double target, double heading, double timeout, util::pidConstants lCons, util::pidConstants acons);
            void odomDrive(double distance, double timeout, double tolerance);
            std::vector<double> moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias);
            void moveTo(util::coordinate target, double timeout, util::pidConstants lConstants, util::pidConstants rConstants, double rotationBias, double rotationScale, double rotationCut);
            void moveToPose(util::bezier curve, double timeout, double lkp, double rkp, double rotationBias);
            void timedSpin(double target, double speed,double timeout);
            void velsUntilHeading(double rvolt, double lvolt, double heading, double tolerance, double timeout);
            void arcTurn(double theta, double radius, double timeout, util::pidConstants cons);
    };
}

void lib::chassis::updatePos(double rx, double ry) //NOLINT
{
    pos.x += rx;
    pos.y += ry;
}

// void lib::chassis::spinTo(double target, double timeout, util::pidConstants constants = util::pidConstants(3.7, 1.3, 26, 0.05, 2.4, 20))
// { 
//   // timers
//   util::timer endTimer;
//   util::timer timeoutTimer;
//   timeoutTimer.start();

//   // basic constants
//   double kP = constants.p;
//   double kI = constants.i;
//   double kD = constants.d;
//   double tolerance = constants.tolerance;
//   double endTime = 2000;

//   // general vars
//   double currHeading = imu.degHeading();
//   double prevHeading = currHeading;
//   double error;
//   double prevError;
//   bool end = false;

//   // eye vars
//   double integral = 0;
//   double integralThreshold = constants.integralThreshold;
//   double maxIntegral = constants.maxIntegral;

//   // dee vars
//   double derivative;
  
//   // pid loop 
//   while (!end)
//   {

//     currHeading = imu.degHeading();
//     int dir = -util::dirToSpin(target,currHeading);

//     //pee
//     error = util::minError(target,currHeading);

//     //eye
//     integral = error <= tolerance ? 0 : error < integralThreshold ? integral + error : integral;

//     if(integral > maxIntegral)
//     {
//       integral = 0;
//     }

//     //dee
//     derivative = error - prevError;
//     prevError = error;

//     //end conditions
//     if (error >= tolerance)
//     {
//       endTimer.start();
//     }

//     end = endTimer.time() >= endTime ? true : timeoutTimer.time() >= timeout ? true : false;

//     // spin motors
//     double rVel = dir * (error*kP + integral*kI + derivative*kD);
//     double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
//     chass.spinDiffy(rVel,lVel);

//     pros::delay(10);
//   }

//   chass.stop('b');
// } 

void lib::chassis::spinTo(double target, double timeout, util::pidConstants constants = util::pidConstants(3.7, 1.3, 26, 0.05, 2.4, 20)) //NOLINT
{ 
  // timers
  util::timer timeoutTimer;

  // general vars
  double currHeading = imu.degHeading();
  double error;
  int dir;
  double vel;

  
  util::pidConstants cons(constants);
  error = util::minError(target,currHeading);

  util::pid pid(cons, error);

  // pid loop 
  while (true)
  {
    //end condition
    if(timeoutTimer.time() >= timeout)
    {
      break;
    }

    //error
    currHeading = imu.degHeading();
    dir = -util::dirToSpin(target,currHeading);
    error = util::minError(target,currHeading);

    //vel
    vel = pid.out(error);

    // spin motors
    chass.spinDiffy(vel * dir,-vel * dir);

    pros::delay(10);
  }
  chass.stop('b');
} 

void lib::chassis::aspin(double target, double timeout, util::pidConstants constants = util::pidConstants(3.7, 1.3, 26, 0.05, 2.4, 20)) //NOLINT
{ 
  // timers
  util::timer timeoutTimer;

  // general vars
  double currHeading = imu.degHeading();
  double error;
  int dir;
  double vel;

  
  util::pidConstants cons(constants);
  error = util::minError(target,currHeading);

  util::pid pid(cons, error);

  // pid loop 
  while (true)
  {
    //end condition
    if(timeoutTimer.time() >= timeout)
    {
      break;
    }

    //error
    currHeading = imu.degHeading();
    dir = -util::dirToSpin(target,currHeading);
    error = util::minError(target,currHeading);

    //vel
    if(error >= cons.tolerance)
    {
      pid.out(error);
      vel = 127;
    }

    else
    {
      vel = pid.out(error);
    }

    // spin motors
    chass.spinDiffy(vel * dir,-vel * dir);

    pros::delay(10);
  }
  chass.stop('b');
}

void lib::chassis::drive(double target, double timeout, util::pidConstants constants) //NOLINT
{
  double error = target;
  util::timer timer;
  util::pid pidController(constants, target);
  chass.reset();

  while(timer.time() < timeout)
  {
    error = target - chass.getRotation();
    chass.spin(pidController.out(error));
  }

  chass.stop('b');
}


// void lib::chassis::drive(double target, double timeout, double tolerance) //NOLINT
// { 
//   // timers
//   util::timer timeoutTimer;

//   // basic constants
//   double kP = 0.3;
//   double kI = 0.2;
//   double kD = 2.4;
//   double endTime = 100000;

//   // general vars
//   double error;
//   double prevError;
//   bool end = false;

//   // eye vars
//   double integral = 0;
//   double integralThreshold = 30;
//   double maxIntegral = 10000;

//   // dee vars
//   double derivative;
  
//   // pid loop 
//   chass.reset();

//   while (!end)
//   {

//     double currRotation = chass.getRotation();

//     //pee
//     error = target - currRotation;
//     // glb::controller.print(0, 0, "%f", error);

//     //eye
//     // integral = error <= tolerance ? 0 : std::abs(error) < integralThreshold ? integral + error : integral;

//     if(integral > maxIntegral)
//     {
//       integral = 0;
//     }
//     //dee
//     derivative = error - prevError;
//     prevError = error;

//     //end conditions

//     end = timeoutTimer.time() >= timeout ? true : false;

//     // spin motors
//     double vel = (error*kP + integral*kI + derivative*kD);
//     chass.spin(vel);

//     pros::delay(10);
//   }
//   chass.stop('b');
// } 

void lib::chassis::autoDrive(double target, double heading, double timeout, util::pidConstants lCons = util::pidConstants(0.3,0.2,2.4,5,30,1000), util::pidConstants acons = util::pidConstants(4, 0.7, 4, 0, 190, 20))
{
  // timers
  util::timer timer;

  // general vars
  double currHeading = imu.degHeading();
  double rot;
  double error;
  double vl;
  double va;
  double dl;
  double dr;
  double sgn = target > 0 ? 1 : -1;

  int dir;

  util::pid linearController(lCons,util::minError(heading, currHeading));
  util::pid angularController(acons,target);

  chass.reset();

  while (true)
  {
    error = util::minError(heading, currHeading);
    if (error < 0.5)
    {
      acons.p = 0;
      angularController.update(acons);
    }

    currHeading = imu.degHeading();
    rot = chass.getRotation();

    va = angularController.out(error);
    vl = linearController.out(target - rot);
    dir = -util::dirToSpin(heading,currHeading);

    if (vl + std::abs(va) > 127)
    {
      vl = 127 - std::abs(va);
    }

    chass.spinDiffy(vl + (dir * va * sgn),  vl - (dir * va * sgn));

    if(timer.time() >= timeout)
    {
      break;
    }

    pros::delay(10);

    // glb::controller.print(0, 0, "%f", util::minError(heading, currHeading));
  }

  chass.stop('b');
}


void lib::chassis::odomDrive(double distance, double timeout, double tolerance) //NOLINT
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
  double heading = imu.radHeading();
  util::coordinate target(sin(2*PI-heading) * dist + pos.x, cos(2*PI-heading) * dist + pos.y);
  double prevRotation;
  double error;
  double prevError;
  bool end = false;

  // eye vars
  double integral = 0;
  double integralThreshold = 30;
//Gerald was here again
  // dee vars
  double derivative;
  
  // pid loop 
  while (!end)
  {
    
    // pee
    error = dist - (dist - util::distToPoint(pos,target));

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
    chass.spin(vel);

    pros::delay(10);
  }
  chass.stop('b');
}  

std::vector<double> lib::chassis::moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias) //NOLINT
{
  double linearError = distToPoint(pos,target);
  double linearVel = linearError*lkp;

  double currHeading =  imu.degHeading(); //0-360
  double targetHeading = absoluteAngleToPoint(pos, target); // -180-180
  // targetHeading = targetHeading >= 0 ? targetHeading + -180 : targetHeading - 180;
  targetHeading = targetHeading >= 0 ? targetHeading :  180 + fabs(targetHeading);  //conver to 0-360

  int dir = -util::dirToSpin(targetHeading,currHeading);

  double rotationError = util::minError(targetHeading,currHeading);
  double rotationVel = rotationError * rkp * dir; 

  // lowers overal speed in porportion to rotationError and rotationBias
  double lVel = (linearVel - (fabs(rotationVel) * rotationBias)) - rotationVel;
  double rVel = (linearVel - (fabs(rotationVel) * rotationBias)) + rotationVel;

  // glb::controller.print(0,0,"(%f, %f)\n", linearError,targetHeading);
  return std::vector<double> {lVel, rVel};
}

void lib::chassis::moveTo(util::coordinate target, double timeout, util::pidConstants lConstants, util::pidConstants rConstants, double rotationBias, double rotationScale, double rotationCut) //NOLINT
{
  //init
  util::timer timeoutTimer;
  double rotationVel, linearVel;
  double linearError = distToPoint(pos,target);
  double initError = linearError;
  double currHeading =  imu.degHeading();
  double targetHeading = absoluteAngleToPoint(pos, target);
  double rotationError = util::minError(targetHeading,currHeading);

  //init pid controllers
  util::pid linearController(lConstants,linearError);
  util::pid rotationController(rConstants,rotationError);

  //maths for scaling the angular p
  double slope = (rConstants.p) / (linearError - rotationCut);
  double initP = rConstants.p;

  while (timeoutTimer.time() < timeout)
  {
    //error
    linearError = distToPoint(pos,target);
    currHeading =  imu.degHeading(); //0-360

    targetHeading = absoluteAngleToPoint(pos, target);
    rotationError = util::minError(targetHeading,currHeading);

    // rConstants.p = slope * log(linearError - lConstants.tolerance + 1);
    rConstants.p = slope * (linearError - initError) + initP;
    rConstants.p = rConstants.p < 0 ? 0 : rConstants.p;
    rotationController.update(rConstants);  
    int dir = -util::dirToSpin(targetHeading,currHeading);
    double cre = cos(rotationError <= 90 ? util::dtr(rotationError) : 0);
    // glb::controller.print(0, 0, "%f,%f", currHeading, targetHeading);

    rotationVel = dir * rotationController.out(rotationError);
    linearVel = cre * linearController.out(linearError);

    double rVel = (linearVel - (fabs(rotationVel) * rotationBias)) + rotationVel;
    double lVel = (linearVel - (fabs(rotationVel) * rotationBias)) - rotationVel;

    chass.spinDiffy(rVel,lVel);
  }

  chass.stop('b');
}

void lib::chassis::moveToPose(util::bezier curve, double timeout, double lkp, double rkp, double rotationBias) //NOLINT
{
  
  // resolution in which to sample points along the curve
  int resolution = 100;

  // util::bezier curve = util::bezier(pos,target,initialBias,finalBias, util::dtr(initialHeading),util::dtr(finalHeading));

  /* creates a look up table so values dont have to be calculated on the fly and dont need to be
  recalculated */
  std::vector<util::coordinate> lut = curve.createLUT(resolution);

  double t;
  double distTraveled = 0;
  double ratioTraveled;
  double curveLength = curve.approximateLength(lut, resolution);

  util::coordinate prevPos = pos;
  util::coordinate targetPos;

  while(1)
  {
    /* approximates dist traveled along the curve by summing the distance between the current
    robot position and the previous robot position */
    distTraveled += util::distToPoint(prevPos, pos);
    prevPos = pos;

    // finds the closest calculated point in the look up table, (rounds up): 0.1 = 1)
    ratioTraveled = distTraveled/curveLength;
    t = std::ceil(ratioTraveled * resolution);

    // bc you count from zero
    targetPos = lut[t-1];

    std::vector<double> velocities = moveToVel(targetPos,0.1,0.1,0.1);
    chass.spinDiffy(velocities[1], velocities[0]);

    // if t reaches the last point
    if (t == lut.size())
    {
      break;
    }
  }
  // moveTo(lut[t], timeout, lkp, rkp, rotationBias);
}


void lib::chassis::timedSpin(double target, double speed,double timeout) //NOLINT
{
  // timers
  util::timer timeoutTimer;

  // general vars
  bool end = false;
//d/dt (2t+sinx)
  double currHeading = imu.degHeading();
  int initDir = -util::dirToSpin(target,currHeading);
  // pid loop 
  while (!end)
  {

    currHeading = imu.degHeading();
    int dir = -util::dirToSpin(target,currHeading);

    double error = util::minError(target,currHeading);
    

    if (initDir != dir)
    {
      end = true;
    }

    end = timeoutTimer.time() >= timeout ? true : end;

    // spin motors

    chass.spinDiffy(dir * speed,- speed*dir);

  }

  chass.stop('b');
}

void lib::chassis::velsUntilHeading(double rvolt, double lvolt, double heading, double tolerance, double timeout) //NOLINT
{
  util::timer timeoutTimer;

  while (true)
  {
    if(util::minError(heading, imu.degHeading()) < tolerance || timeoutTimer.time() >= timeout)
    {
      break;
    }

    chass.spinDiffy(rvolt, lvolt);
  }
}

void lib::chassis::arcTurn(double theta, double radius, double timeout, util::pidConstants cons)
{
  util::timer timer = util::timer(); 
  double curr;
  double currTime;
  double rError;
  double lError;
  double sl;
  double sr;
  double dl;
  double dr;
  double rvel;
  double lvel;
  double vel;
  double ratio;

  sl = theta * (radius + DL);
  sr = theta * (radius + DR);

  theta = util::rtd(theta);
  ratio = sl/sr;
  curr = imu.degHeading();
  rError = util::minError(theta, curr);

  util::pid controller = util::pid(cons, rError);

  while (true)
  {
    curr = imu.degHeading();
    currTime = timer.time();

    int dir = util::dirToSpin(theta,curr);
    vel = controller.out(util::minError(theta, curr)) * dir;

    vel = std::abs(vel) >= 127 ? (127 * util::sign(vel)) : vel;

    rvel = (2 * vel) / (ratio+1);
    lvel = ratio * rvel;

    chass.spinDiffy(rvel, lvel);
    
    if(currTime >= timeout)
    {
      break;
    }
    pros::delay(10);
  }
}

#endif