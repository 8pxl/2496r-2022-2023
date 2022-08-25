#include "global.hpp"
#include "util.hpp"


void odom()
{
    glb::leftEncoder.reset();
    glb::horizEncoder.reset();

    double prevRotation = glb::imu.get_heading();
    double deltaX = 0;
    double deltaY = 0;

    //scale and stuff
    double trackingDiameter = 2.75;
    double trackingCirumfrence = 2.75 * PI;
    double scaleFactor = 5.3625;

    while(1)
    {
        // printf("(%f,%f)\n",glb::pos.x, glb::pos.y);

        // calcualting change in rotation
        double currRotation = glb::imu.get_heading();
        double deltaRotation = util::dtr(currRotation - prevRotation);

        /* when angle difference jumps by more than 300, it can be assumed that it is caused by the imu rotating past
        0 to 360 or from 360 to 0. in order toget the absolute difference in rotation, the mod of bothvalues is taken.
        the fmod() function in cmath is not used, as it does not deal with negative numbers the same way modulo works
        in mathematics
        */

        if (std::abs(deltaRotation) > 300)
        {
            deltaRotation = util::dtr(util::mod(currRotation,360) - util::mod(prevRotation,360));
        }
        prevRotation = currRotation;

        // change in encoder value
        double le = glb::leftEncoder.get_value();
        double he = glb::leftEncoder.get_value();
        double deltaVert = le != 0 ? (trackingCirumfrence/le) : 0  * scaleFactor;
        double deltaHoriz = he != 0 ? (trackingCirumfrence/he) : 0 * scaleFactor;

        if (deltaRotation == 0)
        {
            deltaY = cos(2*PI-currRotation) * le;
            deltaX = sin(2*PI-currRotation) * le;
        }
        else
        {
            // calculating change in relative y
            double sOverTheta = deltaVert / deltaRotation;
            double relativeY = 2*sin(deltaRotation/2) * sOverTheta;

            // calculating change in relative x
            sOverTheta = deltaHoriz/ deltaRotation;
            double relativeX = 2*sin(deltaRotation/2) * sOverTheta;

            // calculing absolute x and y 
            // rotates the coordinate [relativeX, relativeY] to get an absolute position vector
            double rotationOffset = util::dtr(currRotation)+deltaRotation/2;

            double theta = atan2(relativeY, relativeX);
            double radius = sqrt(relativeX*relativeX + relativeY*relativeY);
            theta += rotationOffset;
            deltaX = radius*cos(theta);
            deltaY = radius*sin(theta);

            // double deltaX = relativeX * cos(rotationOffset) - relativeY* sin(rotationOffset);
            // double deltaY = relativeX * sin(rotationOffset) + relativeY* cos(rotationOffset);
        }

        // updating global x and global y
        glb::pos.x -= deltaX;
        glb::pos.y -= deltaY;

        // reset encoders
        glb::horizEncoder.reset();
        glb::leftEncoder.reset();

        pros::delay(10);
    }
}
