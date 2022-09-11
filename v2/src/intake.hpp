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
            bool notIndexing = true;
            while (notIndexing)
            {
                if(flywheel::target - robot::flywheel.getSpeed() > 10)
                {
                    robot::intake.stop("b");
                }

                else
                {
                    robot::intake.spin(-50);
                    pros::delay(80);
                    robot::intake.stop("b");
                    notIndexing = false;
                }
            }

        }
    }

    void spinUntil(double c1, double c2, double speed)
    {
        //c1 is curr color
        while (true)
        {
            if (c1 < c2)
            {
                robot::chass.spin(100);
                if(glb::optical.get_hue() <= c1)
                {
                    robot::intake.spin(speed);
                }

                else if(glb::optical.get_hue() >= c2)
                {
                    // robot::intake.spin(-speed);
                    // pros::delay(150);
                    break;
                }
            }

            else
            {
                robot::chass.spin(100);
                if(glb::optical.get_hue() >= c1)
                {
                    robot::intake.spin(speed);
                }

                else if(glb::optical.get_hue() <= c2)
                {
                    // robot::intake.spin(-speed);
                    // pros::delay(150);
                    break;
                }
            }
        }
    }


    void toggle()
    {

        double initColor = glb::optical.get_hue();
        bool initRed = initColor >= 200 ? false : true;

        double red = 10;
        double blue = 200;
        double speed = 80;

        if(glb::red)
        {   
            if (initRed)
            {
                spinUntil(red, blue, -speed);
            }

            else
            {
                spinUntil(blue, red, speed);
            }
        }

        else
        {   
            if (initRed)
            {
                spinUntil(red, blue, -speed);
            }

            else
            {
                spinUntil(blue, red, -speed);
            }
        }
    }
}






