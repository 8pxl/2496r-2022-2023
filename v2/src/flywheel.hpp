#ifndef __FLYWHEEL__
#define __FLYWHEEL__

#include "global.hpp"


namespace flywheel
{
    double target;
    
    void spin()
    {
        while (true)
        {
            double currSpeed = robot::flywheel.getSpeed();
            double diff = target - currSpeed;

            if (std::abs(diff) < 50)
            {
                robot::flywheel.spin((currSpeed += diff/2) * 127 / 600);
                if (target == 400 || target == 500)
                {
                    glb::controller.rumble("-");
                }
            }

            else
            {
                double voltage = target * 127 / 600;

                if (target > currSpeed)
                {
                    robot::flywheel.spin(voltage * 2);
                }

                else 
                {
                    robot::flywheel.spin(voltage);
                }
            }
            // glb::controller.print(0, 0, "%f", currSpeed);
        }
    }

}

#endif