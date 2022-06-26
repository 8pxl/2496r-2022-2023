#include "vex.h"
#include <cmath>

double pi = 3.141592;
int x=0;
int y=0;

double dtr(double input){
  return(pi*input/180);
}

double mod(double a, double b){
  return fmod(360-std::abs(a), b);
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
    double hDeltaRotation = dtr((currRotation-90) - (prevRotation-90));
    
    if (std::abs(deltaRotation) > 300){
      deltaRotation = dtr(mod(currRotation,360) - mod(prevRotation,360));
    }

    if (std::abs(hDeltaRotation) > 300){
      hDeltaRotation = dtr(mod(currRotation-90,360) - mod(prevRotation-90,360));
    }

    prevRotation = currRotation;
    
    //change in encoder value
    double deltaVert = vertEncoder.position(degrees);
    double deltaHoriz = horizEncoder.position(degrees);
    // calculating change in relative y
    int posY = -1;
    double sOverTheta = deltaVert/deltaRotation;
    double relativeY = sin(deltaRotation) * sOverTheta;
    if (relativeY > 0){
      posY = 1;
    }
    
    // calculating change in relative x
    int posX= -1;
    sOverTheta = deltaHoriz/hDeltaRotation;
    double relativeX = sin(hDeltaRotation) * sOverTheta;
    if (relativeX > 0){
      posX = 1;
    }

    // calculing absolute x and y
    double magnitude = posX * posY * sqrt( (pow(relativeX,2) + pow(relativeY,2)) );
    double deltaX = sin(dtr(currRotation)) * magnitude;
    double deltaY = cos(dtr(currRotation)) * magnitude;

    // printf("%f\n", deltaX);
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
  return atan(px-x/py-y);
}