#include "util.hpp"

void odom() //NOLINT
{
    glb::leftEncoder.reset();
    glb::horizEncoder.reset();

    double prevRotation = glb::imu.get_heading();
    double deltaX = 0;
    double deltaY = 0;

    //scale and stuff
    double trackingDiameter = 2.75;
    double scaleFactor = 5.3625;
    double trackingCirumfrence = (2.75 * PI);
    double horizOffset = 0 * scaleFactor;
    double vertOffset = 0 * scaleFactor;

    while(1)
    {
        // glb::controller.print(1,1,"%f", glb::imu.get_heading());

        // calcualting change in rotation
        double currRotation = robot::imu.degHeading();
        double deltaRotation = currRotation - prevRotation;

        /* when angle difference jumps by more than 300, it can be assumed that it is caused by the imu rotating past
        0 to 360 or from 360 to 0. in order toget the absolute difference in rotation, the mod of bothvalues is taken.
        the fmod() function in cmath is not used, as it does not deal with negative numbers the same way modulo works
        in mathematics
        */

        if (std::abs(deltaRotation) > 300)
        {
            deltaRotation = (util::mod(currRotation,360) - util::mod(prevRotation,360));
        }

        prevRotation = currRotation;

        deltaRotation = util::dtr(deltaRotation);
        currRotation = util::dtr(currRotation);

        // change in encoder value
        double deltaVert = (trackingCirumfrence / 360) * glb::leftEncoder.get_value() * scaleFactor;
        double deltaHoriz = (trackingCirumfrence / 360) * glb::horizEncoder.get_value() * scaleFactor;

        if (deltaRotation == 0)
        {
            deltaY = cos(2*PI-currRotation) * deltaVert;
            deltaX = sin(2*PI-currRotation) * deltaVert;
        }

        else
        {
            // calculating change in relative y
            double sOverTheta = (deltaVert / deltaRotation) + horizOffset;
            double relativeY = 2*sin(deltaRotation/2) * sOverTheta;

            // calculating change in relative x
            sOverTheta = (deltaHoriz / deltaRotation) + vertOffset;
            double relativeX = 2*sin(deltaRotation/2) * sOverTheta;

            // calculing absolute x and y 
            double rotationOffset = currRotation + (deltaRotation/2);
    
            double theta = atan2(relativeY, relativeX);
            double radius = sqrt(relativeX*relativeX + relativeY*relativeY);
            theta -= rotationOffset;
            deltaX = radius*cos(theta);
            deltaY = radius*sin(theta);
        }

        // updating global x and global y
        // glb::controller.print(0,0,"(%f, %f)\n", deltaX,deltaY);
        glb::pos.x -= deltaX;
        glb::pos.y += deltaY;

        // reset encoders
        glb::horizEncoder.reset();
        glb::leftEncoder.reset();

        pros::delay(10);
    }
}
