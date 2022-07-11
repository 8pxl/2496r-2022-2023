#include <cmath>
#include "vex.h"
#include "common.h"
#include <vector>
#include <tuple>

timer timeoutTimer;

class coordinate{
    friend std::ostream& operator<<(std::ostream&, coordinate const&);
    private:
    
    int x;
    int y;
    
    public:
    
    coordinate(int a, int b){
        x = a;
        y = b;
    }
    
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    
};


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

coordinate targetPoint(std::vector<coordinate> path, int lookAhead, int lineLookAhead, int lineIndex){

    coordinate farthestPoint = path[lineLookAhead - 1];
    coordinate targetPoint = coordinate(0,0);
    double closestDist = 1000000000;

    for (int i = lineIndex; i <= lineIndex + lineLookAhead; i++){

        // curr points
        int x1 = path[i].getX();
        int y1 = path[i].getY();
        int x2 = path[i+1].getX();    
        int y2 = path[i+1].getY();

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

            bool s1Valid = s1[0] >= minX && s1[0] <= maxX && s1[1] >= minY && s1[1] <= maxY;
            bool s2Valid = s2[0] >= minX && s2[0] <= maxX && s2[1] >= minY && s2[1] <= maxY;

            double s1Dist = distToPoint(s1[0],s1[1],farthestPoint.getX(),farthestPoint.getY());
            double s2Dist = distToPoint(s2[0],s2[1],farthestPoint.getX(), farthestPoint.getY());

            if (s1Valid and s1Dist < closestDist){
                targetPoint = coordinate(s1[0],s1[1]);
                closestDist = s1Dist;
            }

            if (s2Valid and s2Dist < closestDist){
                targetPoint = coordinate(s2[0],s2[1]);
                closestDist = s2Dist;
            }

        }

    } 

    return(targetPoint);
}



bool pointInCircle(coordinate point, int radius){
    return distToPoint(x, y, point.getX(), point.getY()) < radius ?  true : false;
}

void moveToPurePursuit(std::vector<coordinate> path, int lookAhead, int lineLookAhead, int finalTimeout){

    bool targetReached = false;
    
    double lVel;
    double rVel;

    int lineIndex = 0;
    
    while (!targetReached){

        if (pointInCircle(path[lineIndex + lineLookAhead], lookAhead)){
            lineIndex += 1;
        }

        if (pointInCircle(path.back(), lookAhead)){
            targetReached = true;
        }

        coordinate target = targetPoint(path,lookAhead, lineLookAhead, lineIndex);

        std::tie(lVel,rVel) = moveToVel(target);
        spinChassis(lVel, rVel);
    }

   moveTo(path.back(), finalTimeout);
}   