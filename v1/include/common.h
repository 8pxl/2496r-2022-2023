#include <iostream>
#include "vex.h"

#define PI 3.14159

extern int x;
extern int y;

class coordinate;


double dtr(double input);
double mod(double a, double b);
double rtd(double input);
int sign(double input);
double distToPoint(int x, int y, int px, int py);
double absoluteAngleToPoint(int px, int py);
int dirToSpin(double target, double unscaled);
std::tuple<int, int> moveToVel(coordinate target);
void spinChassis(double lVel,double rVel);
void stopChassis();
void moveTo(coordinate target, double timeout);

