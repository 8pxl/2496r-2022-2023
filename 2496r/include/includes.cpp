#include "vex.h"
#include <cmath>

float averageRpm(vex::motor m1, vex::motor m2, vex::motor m3, vex::motor m4){
  return ( std::abs(m1.velocity(rpm)) + std::abs(m2.velocity(rpm)) + std::abs(m3.velocity(rpm)) + std::abs(m4.velocity(rpm)) / 4 );
}

float fPid(float target){
  float kp = 1.3;
  int error = target - averageRpm();
  return error*kp;
}

void targetUp(){
  int offset = 50;
  rpmTarget = rpmTarget+offset;
}

void targetDown(){
  int offset = 50;
  rpmTarget = rpmTarget- offset;
}