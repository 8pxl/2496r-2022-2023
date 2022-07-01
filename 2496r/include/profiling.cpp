#include <algorithm>
#include <cmath>
#include "vex.h"
#include "common.h"
#include <list>
#include <iostream>
#include <iterator>
#include <vector>


// std::ostream &operator<<(std::ostream &os, coordinate const &m) { 
//     return os << m.x;
// }

coordinate p1[] = {coordinate(276,693), coordinate(370,466), coordinate(583,339), coordinate(516,178), coordinate(303,200)};

std::vector<coordinate> one(p1, p1 + sizeof(p1)/ sizeof(coordinate));

// for (auto v : one)
//     std::cout << v << "\n";

int sign(double input){
    return(input > 0 ? -1: 1);
}

void purePursuit(std::vector<coordinate> path, int lookAhead){
    while (true){
        // curr points
        int x1 = path[0].getX();
        int y1 = path[0].getY();
        int x2 = path[1].getX();
        int y2 = path[1].getY();
        int targetPoint[2] = {x2, y2};

        // offset to origin
        int ox1 = x1 - x;
        int oy1 = y1 - y;
        int ox2 = x2 - x;
        int oy2 = y2 - y;

        // set up to find intersection using this method 
        // https://mathworld.wolfram.com/Circle-LineIntersection.html
        int dx = ox2-ox1;
        int dy = oy2-oy1;
        double dr = sqrt(pow(dx,2)+pow(dy,2));
        int D = ox1*oy2 - ox2 - oy1;

        double discriminant = pow(lookAhead,2)  *  pow(dr,2) - pow(D,2);

        int solution = -1;
        double solutions[2][2];

        // calculate solutions
        if (discriminant >= 0){
            double sDiscriminant = sqrt(discriminant);
            double dxdy = D * dy;
            double dxdx = D*dx;
            double sdyxdxxsd = sign(dy) * dx * sDiscriminant;
            double dr2 = pow(dr,2);
            double adyxsd = std::abs(dy) * sDiscriminant;

            int minX = std::min(x1,x2);
            int maxX = std::max(x1,x2);
            int minY = std::min(y1,y2);
            int maxY = std::max(y1,y2);

            double sx1 = (dxdy + sdyxdxxsd) / dr2;
            double sy1 = (-dxdx + adyxsd) / dr2;
            double sx2 = (dxdy - sdyxdxxsd) / dr2;
            double sy2 = (-dxdx - adyxsd) / dr2;

            double s1[2]= {sx1+x,sy1+y};
            double s2[2] = {sx2+x,sy2+y};

            double solutions[2][2] = { 
            {s1[0], s1[1]},
            {s2[0], s2[1]}
            };

            bool s1Valid = s1[0] >= minX && s1[0] <= maxX && s1[1] >= minY && s1[1] <= maxY;
            bool s2Valid = s2[0] >= minX && s2[0] <= maxX && s2[1] >= minY && s2[1] <= maxY;

            if (s1Valid){
                solution = 0;
            }
            if (s2Valid){
                solution=1;  
            }
            if (s1Valid && s2Valid){
                if(distToPoint(s1[0],s1[1],targetPoint[0],targetPoint[1]) > distToPoint(s2[0],s2[1],targetPoint[0], targetPoint[1])){
                    solution = 1;
                }
                else{
                solution = 0;
                }
            }

        }

        // move to target point

        // constants;
        double lkp = 0.1;
        double rkp = 0.1;

        double currHeading = imu.heading();

        double tx = solutions[solution][0];
        double ty = solutions[solution][1];

        // linear stuffs.
        double linearError = distToPoint(x,y,tx, ty);
        double linearVel = linearError*lkp;

        // rotation stuffs.
        double target = rtd(absoluteAngleToPoint(tx, ty));
        int dir = dirToSpin(target,currHeading);
        double rotationError = std::abs(target - currHeading);
        double rotationVel = rotationError * rkp * dir;

        //velocities
        double lVel = linearVel - rotationVel;
        double rVel = linearVel + rotationVel;

        frontLeft.spin(fwd, lVel, pct);
        backLeft.spin(fwd,lVel, pct);
        frontRight.spin(fwd,rVel, pct);
        backRight.spin(fwd, rVel, pct)
    }
}   

