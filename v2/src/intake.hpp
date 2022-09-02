#include "global.hpp"
#include "flywheel.hpp"

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
                if(flywheel::target - robot::flywheel.getSpeed() > 30)
                {
                    robot::intake.stop("b");
                }

                else
                {
                    robot::intake.spin(-50);
                }
            }

        }
    }

    void toggle()
    {
        if(glb::red)
        {   
            while (true)
            {
                if(glb::optical.get_hue() >= 200)
                {
                    robot::intake.spin(80);
                }

                else if(glb::optical.get_hue() <= 10)
                {
                    robot::intake.spin(-80);
                    pros::delay(500);
                    break;
                }
            }
        }

        else
        {   
            while (true)
            {
                if(glb::optical.get_hue() >= 200)
                {
                    robot::intake.spin(-80);
                    pros::delay(500);
                    break;
                }

                else if(glb::optical.get_hue() <= 10)
                {
                    robot::intake.spin(80);
                }
            }
        }
    }
}