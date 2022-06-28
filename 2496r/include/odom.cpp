#include <cmath>
#include "vex.h"
#include "common.hpp"


void odom(){
  vertEncoder.setPosition(0,deg);
  horizEncoder.setPosition(0,deg);
  double prevRotation = imu.heading();
  
  while(1){
    printf("(%i,%i)\n",x,y);

    // calcualting change in rotation
    double currRotation = imu.heading();
    double deltaRotation = dtr(currRotation - prevRotation);
    
    /* when angle difference jumps by more than 300, it can be assumed that it is caused by the imu rotating past
    0 to 360 or from 360 to 0. in order to get the absolute difference in rotation, the mod of bothvalues is taken.
    the fmod() function in cmath is not used, as it does not deal with negative numbers the same way modulo works
    in mathematics
    */

    if (std::abs(deltaRotation) > 300){
      deltaRotation = dtr(mod(currRotation,360) - mod(prevRotation,360));
    }

    prevRotation = currRotation;
    
    // change in encoder value
    double deltaVert = vertEncoder.position(degrees);
    double deltaHoriz = horizEncoder.position(degrees);

    // calculating change in relative y
    double sOverTheta = deltaVert/deltaRotation;
    double relativeY = 2 * sin(deltaRotation/2) * sOverTheta;
    
    // calculating change in relative x
    sOverTheta = deltaHoriz/deltaRotation;
    double relativeX = 2* sin(deltaRotation/2) * sOverTheta;

    // calculing absolute x and y 
    // rotates the vector [relativeX, relativeY] to get an absolute position vector
    double rotationOffset = dtr(currRotation)+deltaRotation/2;
    double deltaX = relativeX * cos(rotationOffset) - relativeY* sin(rotationOffset);
    double deltaY = relativeX * sin(rotationOffset) + relativeY* cos(rotationOffset);

    // updating global x and global y
    x += deltaX;
    y += deltaY;

    // reset encoders
    horizEncoder.setPosition(0,deg);
    vertEncoder.setPosition(0,deg);

    vex::task::sleep(10);
  }
}

double distToPoint(int px, int py){
  return sqrt( (px-x)^2 + (py-y)^2 );
}

double absoluteAngleToPoint(int px, int py){
  return atan2(px-x,py-y);
}