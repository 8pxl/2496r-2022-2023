#include "global.hpp"
#include "flywheel.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"

namespace intake
{
    void index(int num)
    {
        for (int i = 0; i < num; i++)
        {
            robot::intake.spin(-50);
            pros::delay(250);
            robot::intake.stop("b");
            pros::delay(100);
        }
    }

    void waitIndex(int speed)
    {
        while (true)
        {
            if(robot::flywheel.getSpeed() >= speed)
            {
                robot::intake.spin(-50);
                pros::delay(250);
                robot::intake.stop("b");
                pros::delay(100);
                return;
            }
        }
    }

    // void index(int num)
    // {
    //     for (int i = 0; i < num; i++)
    //     {
    //         while (true)
    //         {
    //             glb::controller.print(0, 0, "%f", flywheel::target - robot::flywheel.getSpeed());
    //             if(flywheel::target - robot::flywheel.getSpeed() > 5)
    //             {
    //                 robot::intake.stop("b");
    //             }

    //             else
    //             {
    //                 // robot::intake.spinDist(120, 50 , "b");
    //                 robot::intake.spin(-80);
    //                 pros::delay(130);
    //                 robot::intake.stop("b");
    //                 break;
    //             }
    //             pros::delay(10);
    //         }

    //     }
    // }

    void spinUntil(double color, double speed)
    {

        if(color == 200)
        {
            while(true)
            {
                robot::intake.spin(speed);

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
                robot::intake.spin(speed);

                if( glb::optical.get_hue() < color)
                {
                    break;
                }
            }
        }
    }


    void toggle(double timeLimit = 3000)
    {
        glb::optical.set_led_pwm(100);
        robot::chass.spin(85);
        pros::delay(100);
        double initColor = glb::optical.get_hue();
        // glb::controller.print(1, 1, "%f", glb::optical.get_hue());
        bool initRed = initColor >= 60 ? false : true;

        double red = 10;
        double blue = 200;
        double speed = 90;

        if (!glb::red)
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

        glb::optical.set_led_pwm(0);
        robot::chass.stop("b");
    }

}






