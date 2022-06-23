#include "vex.h"
#include <cmath>

double pi = 3.141592;
int x=0;
int y=0;

double dtr(double input){
  return(pi*input/180);
}

void odom(){

  double prevRotation = imu.heading();

  vertEncoder.setPosition(0,deg);
  horizEncoder.setPosition(0,deg);
  vex::task::sleep(200);
  
  while(1){
    printf("%i",x);
    printf("%c",',');
    printf("%i\n",y);
    
    double currRotation = imu.heading();

    // calcualting change in rotation
    double deltaRotation = dtr(currRotation - prevRotation);
    
    if (std::abs(deltaRotation) > 300){
      deltaRotation = dtr(fmod(currRotation,360) - fmod(prevRotation,360));
    }

    prevRotation = currRotation;
    
    //change in encoder value
    double deltaVert = vertEncoder.position(degrees);
    double deltaHoriz = horizEncoder.position(degrees);

    // calculating change in relative y
    double sOverTheta = deltaVert/deltaRotation;
    double relativeY = sin(deltaRotation) * sOverTheta;
    
    // calculating change in relative x
    sOverTheta = deltaHoriz/deltaRotation;
    double relativeX = sin(deltaRotation) * sOverTheta;

    // calculing absolute x
    double magnitude = sqrt( (pow(relativeX,2) + pow(relativeY,2)) );
    double deltaX = sin(dtr(currRotation)) * magnitude;
    double deltaY = cos(dtr(currRotation)) * magnitude;

    // updating global x and global y
    x += deltaX;
    y += deltaY;

    // reset encoders
    horizEncoder.setPosition(0,deg);
    vertEncoder.setPosition(0,deg);

    // printf("(%i,%i)\n",x,y);

    vex::task::sleep(20);
  }
}

double distToPoint(int px, int py){
  return sqrt( (px-x)^2 + (py-y)^2 );
}

double absoluteAngleToPoint(int px, int py){
  return 90 - imu.heading();
}
