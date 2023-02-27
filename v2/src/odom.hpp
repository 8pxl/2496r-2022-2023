#include "global.hpp"
#include "util.hpp"


void odom()
{
    glb::leftEncoder.reset();
    glb::horizEncoder.reset();

    double prevRotation;
    double currRotation;
    double deltaRotation;
    double deltaVert;
    double deltaHoriz;
    double sOverTheta;
    double relativeY;
    double relativeX;
    double rotationOffset;
    double theta;
    double radius;
    double deltaX;
    double deltaY;

    //scale and stuff
    double trackingDiameter = 2.75;
    double scaleFactor = 5.3625;
    double trackingCirumfrence = (2.75 * PI);
    double horizOffset = 4.39 * scaleFactor;
    double vertOffset = 2.13 * scaleFactor;

    while(1)
    {
        currRotation = robot::imu.degHeading();
        deltaRotation = util::minError(currRotation, prevRotation) * util::dirToSpin(currRotation,prevRotation);

        prevRotation = currRotation;

        deltaRotation = util::dtr(deltaRotation);
        currRotation = util::dtr(currRotation);

        // change in encoder value
        deltaVert = (trackingCirumfrence / 360) * glb::leftEncoder.get_value() * scaleFactor;
        deltaHoriz = (trackingCirumfrence / 360) * glb::horizEncoder.get_value() * scaleFactor;

        if (deltaRotation == 0)
        {
            deltaY = cos(2*PI-currRotation) * deltaVert;
            deltaX = sin(2*PI-currRotation) * deltaVert;
        }

        else
        {
            // calculating change in relative y
            sOverTheta = (deltaVert / deltaRotation) + horizOffset;
            relativeY = 2*sin(deltaRotation/2) * sOverTheta;

            // calculating change in relative x
            sOverTheta = (deltaHoriz / deltaRotation) + vertOffset;
            relativeX = 2*sin(deltaRotation/2) * sOverTheta;

            // calculing absolute x and y 
            rotationOffset = currRotation + (deltaRotation/2);
    
            theta = atan2(relativeY, relativeX);
            radius = sqrt(relativeX*relativeX + relativeY*relativeY);
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
