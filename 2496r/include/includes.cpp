#include "vex.h"
#include <cmath>
#include "common.h"

float averageRpm(vex::motor m1, vex::motor m2, vex::motor m3, vex::motor m4){
  return ( std::abs(m1.velocity(rpm)) + std::abs(m2.velocity(rpm)) + std::abs(m3.velocity(rpm)) + std::abs(m4.velocity(rpm)) / 4 );
}

double dtr(double input){
  return(PI*input/180);
}

double rtd(double input){
  return(input*180/PI);
}

double mod(double a, double b){
  return fmod(360-std::abs(a), b);
}

// float fPid(float target){
//   float kp = 1.3;
//   int error = target - averageRpm();
//   return error*kp;
// }

// void targetUp(){
//   int offset = 50;
//   rpmTarget = rpmTarget+offset;
// }

// void targetDown(){
//   int offset = 50;
//   rpmTarget = rpmTarget- offset;
// }