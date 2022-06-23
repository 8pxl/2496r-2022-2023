#include "vex.h"

int rpmTarget = 550;
float averageRpm(){
  return ( (leftFly.velocity(rpm) + abs(rightFly.velocity(rpm))) /2 );
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