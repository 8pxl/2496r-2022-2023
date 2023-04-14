#ifndef __CHASSIS__
#define __CHASSIS__

// - chassis specific macros 
#define DL 368.2
#define DR -362
#define MAXSPEED 0.5672320069
#define MAXACCEL 0.5672320069 / 70

#include "global.hpp"

namespace chas
{
  class follower;
  void spinTo(double target, double timeout, util::pidConstants constants); 
  void drive(double target, double timeout, double tolerance, double max); //target = encoder units 
  void driveAngle(double target, double heading, double timeout, util::pidConstants lCons, util::pidConstants acons);
  void odomDrive(double distance, double timeout, double tolerance);
  std::vector<double> moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias);
  void moveTo(util::coordinate target, double timeout, util::pidConstants lConstants, util::pidConstants rConstants, double rotationBias, double rotationScale, double rotationCut);
  void moveToPose(util::bezier curve, double timeout, double lkp, double rkp, double rotationBias);
  void timedSpin(double target, double speed,double timeout);
  void velsUntilHeading(double rvolt, double lvolt, double heading, double tolerance, double timeout);
  void arcTurn(double theta, double radius, double timeout, int dir, util::pidConstants cons); 
  std::vector<double> trapezoidalProfile(double dist, double maxSpeed, double accel);
  void contradrive(double target,util::pidConstants profileConstants, double sf, util::pidConstants constants, int endTimeout);
  void contraturn(double target, util::pidConstants profileConstants, double sf, util::pidConstants constants, int endTimeout);
}

  // class A
  // {
  //   private:
  //     int num;
  //   public:
  //     A(int n) : num(n) {}
  //     A() = default;
  //     void setNum(int n)
  //     {
  //       num = n;
  //     }
  // };

  // class B
  // {
  //   private:
  //     A var;

  //   public:
  //     B(int num)
  //     {
  //       var.setNum(num);
  //     }
  // };

class chas::follower
{
  private:
    std::vector<double> profile;
    util::pid controller;
    util::pidConstants constants;
    double sf;
    double kv;
    double negbias;
    util::timer timer;

  public:
    follower(std::vector<double> profile, util::pidConstants constants, double kv, double sf, double negbias = 30) : profile(profile), kv(kv), constants(constants), sf(sf), negbias(negbias)
    { 
      controller.update(constants);
    }
    
    double out(double curr)
    {
      int t = timer.time() / 10;
      double rpm = profile[t] * sf;
      double error = curr - rpm;
      if (error > 0)
      {
        error += error/(1);
      }
      printf("%f,%f,", curr, rpm);
      return rpm * kv + controller.out(-error);
    }

    void start()
    {
      timer.start();
    }
};


// void chas::spinTo(double target, double timeout, util::pidConstants constants = util::pidConstants(3.7, 1.3, 26, 0.05, 2.4, 20))
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
//   double currHeading = robot::imu.degHeading();
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

//     currHeading = robot::imu.degHeading();
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

//     if(timeoutTimer.time()>= timeout)
//     {
//       break;
//     }

//     // spin motors
//     double rVel = dir * (error*kP + integral*kI + derivative*kD);
//     double lVel = dir * -1 * (error*kP + integral*kI + derivative*kD);
//     robot::chass.spinDiffy(rVel,lVel);

//     pros::delay(10);
//     glb::controller.print(0, 0, "%f", error);
//     // glb::controller.print(0, 0, "%f", integral);
//   }
//   robot::chass.stop("b");
// } 


void chas::spinTo(double target, double timeout, util::pidConstants constants = util::pidConstants(3.7, 1.3, 26, 0.05, 2.4, 20))
{ 
  util::timer timeoutTimer;
  util::pid pid(constants, 0);
  double currHeading;
  double vel;

  while (timeoutTimer.time() <= timeout)
  {
    currHeading = robot::imu.degHeading();
    vel = util::dirToSpin(target,currHeading) * pid.out(util::minError(target,currHeading));
    robot::chass.spinDiffy(-vel,vel);
    pros::delay(10);
  }
  robot::chass.stop("b");
} 


// void chas::spinTo(double target, double timeout, double tolerance)
// { 
//   // timers
//   util::timer timeoutTimer;

//   // basic constants
//   double kP = 2.1;
//   double kI = 0.1;
//   double kD = 7;

//   // general vars
//   double currHeading = robot::imu.degHeading();
//   double error;
//   int dir;
//   double vel;

//   // eye vars
//   double integral = 0;
//   double integralThreshold = 10;

//   // dee vars
//   double derivative;
  
//   util::pidConstants constants(kP,kI,kD, tolerance, integralThreshold);
//   error = util::minError(target,currHeading);
//   util::pid pid(constants, error);


//   // pid loop 
//   while (true)
//   {
//     //end condition
//     if(timeoutTimer.time() >= timeout)
//     {
//       break;
//     }

//     //error
//     currHeading = robot::imu.degHeading();
//     dir = -util::dirToSpin(target,currHeading);
//     error = util::minError(target,currHeading);

//     //vel
//     vel = pid.out(error);

//     // spin motors
//     robot::chass.spinDiffy(vel * dir,-vel * dir);

//     pros::delay(10);
//   }
//   robot::chass.stop("b");
// } 

void chas::drive(double target, double timeout, double tolerance, double max = 127)
{ 
  // timers
  util::timer timeoutTimer;

  // basic constants
  double kP = 0.3;
  double kI = 0.2;
  double kD = 2.4;
  double endTime = 100000;

  // general vars
  double error;
  double prevError;
  bool end = false;

  // eye vars
  double integral = 0;
  double integralThreshold = 30;
  double maxIntegral = 10000;

  // dee vars
  double derivative;
  
  // pid loop 
  robot::chass.reset();

  while (!end)
  {

    double currRotation = robot::chass.getRotation();

    //pee
    error = target - currRotation;
    // glb::controller.print(0, 0, "%f", error);

    //eye
    // integral = error <= tolerance ? 0 : std::abs(error) < integralThreshold ? integral + error : integral;

    if(integral > maxIntegral)
    {
      integral = 0;
    }
    //dee
    derivative = error - prevError;
    prevError = error;

    //end conditions

    end = timeoutTimer.time() >= timeout ? true : false;

    // spin motors
    double rVel = (error*kP + integral*kI + derivative*kD);

    if(rVel > max)
    {
      rVel = max;
    }

    robot::chass.spinDiffy(rVel,rVel);

    pros::delay(10);
    // glb::controller.print(0, 0, "%f", error);
  }
  robot::chass.stop("b");
} 

void chas::driveAngle(double target, double heading, double timeout, util::pidConstants lCons = util::pidConstants(0.3,0.2,2.4,5,30,1000), util::pidConstants acons = util::pidConstants(4, 0.7, 4, 0, 190, 20))
{
  // timers
  util::timer timer;

  // general vars
  double currHeading = robot::imu.degHeading();
  double rot;
  double error;
  double vl;
  double va;
  double dl;
  double dr;
  double sgn = target > 0 ? 1 : -1;

  int dir;

  util::pid linearController(lCons, 0);
  util::pid angularController(acons,target);

  robot::chass.reset();

  while (timer.time() <= timeout)
  {
    error = util::minError(heading, currHeading);
    if (error < 0.5)
    {
      acons.p = 0;
      angularController.update(acons);
    }

    currHeading = robot::imu.degHeading();
    rot = robot::chass.getRotation();

    va = angularController.out(error);
    vl = linearController.out(target - rot);
    // vl = 0;
    dir = -util::dirToSpin(heading,currHeading);

    if (vl + std::abs(va) > 127)
    {
      vl = 127 - std::abs(va);
    }

    robot::chass.spinDiffy(vl + (dir * va * sgn),  vl - (dir * va * sgn));

    pros::delay(10);

    glb::controller.print(0, 0, "%f", util::minError(heading, currHeading));
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
  robot::chass.stop('.');
}  

std::vector<double> chas::moveToVel(util::coordinate target, double lkp, double rkp, double rotationBias)
{
  double linearError = distToPoint(glb::pos,target);
  double linearVel = linearError*lkp;

  double currHeading =  robot::imu.degHeading(); //0-360
  double targetHeading = absoluteAngleToPoint(glb::pos, target); // -180-180
  targetHeading = targetHeading >= 0 ? targetHeading :  180 + fabs(targetHeading); 

  int dir = -util::dirToSpin(targetHeading,currHeading);

  double rotationError = util::minError(targetHeading,currHeading);
  double rotationVel = rotationError * rkp * dir; 

  // lowers overal speed in porportion to rotationError and rotationBias
  double lVel = (linearVel - (fabs(rotationVel) * rotationBias)) - rotationVel;
  double rVel = (linearVel - (fabs(rotationVel) * rotationBias)) + rotationVel;

  return std::vector<double> {lVel, rVel};
}

void chas::moveTo(util::coordinate target, double timeout, util::pidConstants lConstants, util::pidConstants rConstants, double rotationBias, double rotationScale, double rotationCut)
{
  //init
  util::timer timeoutTimer;
  double rotationVel, linearVel;
  double linearError = distToPoint(glb::pos,target);
  double initError = linearError;
  double currHeading =  robot::imu.degHeading();
  double targetHeading = absoluteAngleToPoint(glb::pos, target);
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
    linearError = distToPoint(glb::pos,target);
    currHeading =  robot::imu.degHeading(); //0-360

    targetHeading = absoluteAngleToPoint(glb::pos, target);
    rotationError = util::minError(targetHeading,currHeading);

    rConstants.p = slope * (linearError - initError) + initP;
    rConstants.p = rConstants.p < 0 ? 0 : rConstants.p;
    rotationController.update(rConstants);  
    int dir = -util::dirToSpin(targetHeading,currHeading);
    double cre = cos(rotationError <= 90 ? util::dtr(rotationError) : PI/2);

    rotationVel = dir * rotationController.out(rotationError);
    linearVel = cre * linearController.out(linearError);

    double rVel = (linearVel - (fabs(rotationVel) * rotationBias)) + rotationVel;
    double lVel = (linearVel - (fabs(rotationVel) * rotationBias)) - rotationVel;

    robot::chass.spinDiffy(rVel,lVel);
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
  // moveTo(lut[t], timeout, lkp, rkp, rotationBias);
}

void chas::timedSpin(double target, double speed,double timeout)
{
  // timers
  util::timer timeoutTimer;

  // general vars
  bool end = false;

  double currHeading = robot::imu.degHeading();
  int initDir = -util::dirToSpin(target,currHeading);
  // pid loop 
  while (!end)
  {

    currHeading = robot::imu.degHeading();
    int dir = -util::dirToSpin(target,currHeading);

    double error = util::minError(target,currHeading);
    

    if (initDir != dir)
    {
      end = true;
    }

    end = timeoutTimer.time() >= timeout ? true : end;

    // spin motors

    robot::chass.spinDiffy(dir * speed,- speed*dir);

  }

  robot::chass.stop("b");
}

void chas::velsUntilHeading(double rvolt, double lvolt, double heading, double tolerance, double timeout)
{
  util::timer timeoutTimer;

  while (true)
  {
    if(util::minError(heading, robot::imu.degHeading()) < tolerance || timeoutTimer.time() >= timeout)
    {
      break;
    }

    robot::chass.spinDiffy(rvolt, lvolt);
  }
}

void chas::arcTurn(double theta, double radius, double timeout, int dir, util::pidConstants cons)
{
  util::timer timer;
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
  curr = glb::imu.get_heading();
  util::pid controller(cons, 1000);

  while (timer.time() < timeout)
  {
    curr = glb::imu.get_heading();
    vel = controller.out(util::minError(theta, curr)) * util::dirToSpin(theta,curr);
    vel = std::abs(vel) >= 127 ? (127 * util::sign(vel)) : vel;
    rvel = (2 * vel) / (ratio+1);
    lvel = ratio * rvel;

    if(util::sign(dir) == 1)
    {
      robot::chass.spinDiffy(rvel, lvel);
    }

    else
    {
      robot::chass.spinDiffy(-lvel, -rvel);
    }

    pros::delay(10);

  }
  robot::chass.stop("b");
}

std::vector<double> chas::trapezoidalProfile(double dist, double maxSpeed = MAXSPEED, double accel = MAXACCEL)
{
  double max = std::min(std::sqrt(dist * accel), maxSpeed);
  double accelTime = max / accel;
  double accelDist = (accel / 2) * std::pow(accelTime, 2);
  double coastDist = dist - (2 * accelDist);
  double coastTime = coastDist / max;
  double totalTime = 2 * accelTime + coastTime;
  glb::controller.print(1,1,"%f", maxSpeed);
  double vel = 0;
  double diff;
  std::vector<double> profile;

  for (int i = 0; i < std::ceil(totalTime); i++)
  {
    if (i < std::floor(accelTime))
    {
      profile.push_back(vel);
      vel += accel;
    }

    else if (i < coastTime + accelTime)
    {
      profile.push_back(max);
    }

    else
    {
      profile.push_back(vel);
      vel -= accel;
    }
  }
  //second pass
  int size = profile.size();
  double traveled = std::reduce(profile.begin(), profile.end());
  diff = traveled - dist;
  double adj = diff / (size / 5);
  int num = std::ceil(size /5);
  for (int i = 0; i < num; i++)
  {
    if (profile[size - i - 1] > adj)
    {
        profile[size - i - 1] -= adj;
    }

    else
    {
      num += 1;
    }
    // printf("%f\n", profile[size-i]);
  }
  // }

  return profile;
}


void chas::contradrive(double target, util::pidConstants profileConstants, double sf, util::pidConstants constants, int endTimeout)
{
  //kv, rpm -> voltage
  //sf, in/ms -> rpm
  double targetRot = target * 58.76490206;
  std::vector<double> profile = chas::trapezoidalProfile(target);
  robot::chass.reset();
  chas::follower controller(profile,profileConstants, profileConstants.kv, sf);
  util::pid pid(constants, 0);

  // glb::controller.print(1,1, "%f", target);
  // glb::controller.print(1,1, "%f", target);
  // printf("%f", profile.size());

  for (int i = 0; i < profile.size(); i++)
  {
    double curr = robot::chass.getSpeed();
    // printf("%f, ", profile[i] * sf * profileConstants.kv);
    robot::chass.spin(controller.out(robot::chass.getSpeed()));
    pros::delay(10);
  }

  util::timer timer;

  while(timer.time() < endTimeout)
  {
    // glb::controller.print(1,1,"%f", robot::chass.getRotation());
    robot::chass.spin(pid.out(targetRot - robot::chass.getRotation()));
    pros::delay(10);
  }

  robot::chass.stop("b");
  robot::chass.stop("b");
}

void chas::contraturn(double target, util::pidConstants profileConstants, double sf, util::pidConstants constants, int endTimeout)
{
  //kv, rpm -> voltage
  //sf, deg/ms -> rpm
  std::vector<double> profile = chas::trapezoidalProfile(target);
  robot::chass.reset();
  chas::follower controller(profile,profileConstants, profileConstants.kv, sf);
  util::pid pid(constants, 0);

  for (int i = 0; i < profile.size(); i++)
  {
    double volts = controller.out(robot::chass.getSpeed());
    robot::chass.spinDiffy(volts, -volts);
  }

  util::timer timer;

  while(timer.time() < endTimeout)
  {
    double curr = robot::imu.degHeading();
    double volts = pid.out(util::minError(target, curr)) * util::dirToSpin(target, curr);
    robot::chass.spinDiffy(volts, -volts);
  }

  robot::chass.stop("b");
}

#endif