#include "global.hpp"
#include "flywheel.hpp"
#include "pros/rtos.hpp"

namespace intake
{
    // void index(int num)
    // {
    //     for (int i = 0; i < num; i++)
    //     {
    //         robot::intake.spin(-50);
    //         pros::delay(250);
    //         robot::intake.stop("b");
    //         pros::delay(100);
    //     }
    // }

    void index(int num)
    {
        for (int i = 0; i < num; i++)
        {
            while (true)
            {
                if(flywheel::target - robot::flywheel.getSpeed() > 10)
                {
                    robot::intake.stop("b");
                }

                else
                {
                    robot::intake.spinDist(120, 50 , "b");
                    break;
                }
            }

        }
    }

    void spinUntil(double color, double speed)
    {
        robot::chass.spin(50);
        
        if(color == 200)
        {
            while(true)
            {
                robot::intake.spin(127);

                if( glb::optical.get_hue() > color)
                {
                    break;
                }
            }
        }

        else
        {
            while(true)
            {
                robot::intake.spin(127);

                if( glb::optical.get_hue() < color)
                {
                    break;
                }
            }
        }
        robot::chass.stop("b");
    }


    void toggle()
    {

        double initColor = glb::optical.get_hue();
        bool initRed = initColor >= 200 ? false : true;

        double red = 10;
        double blue = 200;
        double speed = 80;

        if (glb::red)
        {
            if(initRed)
            {
                spinUntil(blue, 127);
                spinUntil(red, speed);
            }

            else
            {
                spinUntil(red, speed);
            }
            
        }

        else 
        {
            if(initRed)
            {
                spinUntil(blue, speed);
            }

            else
            {
                spinUntil(red, 127);
                spinUntil(blue, speed);
            }
        }
    }
}






