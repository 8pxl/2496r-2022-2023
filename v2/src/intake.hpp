#include "global.hpp"
#include "flywheel.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"

namespace intake
{
    util::timer inRange;

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
    
    void hardToggle()
    {
        robot::intake.spin(127);
        pros::delay(300);
        robot::chass.spin(90);
        pros::delay(180);
        robot::intake.stop("C");
        // robot::chass.spin(-90);
        // pros::delay(300);
        robot::chass.stop("b");
    }
    void waitIndex(int num, int tolerance = 5, int ff = -1, int time = 50, int ffTime = 0)
    {
        for (int i = 0; i < num; i++)
        {
            while (true)
            {
                if(flywheel::gError < tolerance)
                {

                    if(inRange.time() >= time)
                    {
                        if (i == num-1)
                        {
                            flywheel::ff = ff;
                            pros::delay(ffTime);
                            robot::intake.spin(-80);
                            pros::delay(200);
                            inRange.start();
                        }

                        else
                        {
                            flywheel::ff = ff;
                            pros::delay(ffTime);
                            robot::intake.spin(-50);        
                            pros::delay(200);
                            robot::intake.stop("b");
                            pros::delay(250);
                            inRange.start();
                        }

                        break;
                    }
                }
                
                else
                {
                    inRange.start();
                }
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

    void spinUntil(double color, double speed, util::timer timer, double timeout)
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

                if(timer.time() >= timeout)
                {
                    return;
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
                
                if(timer.time() >= timeout)
                {
                    return;
                }
            }
        }
    }

    // void toggle(bool ym, double timeLimit = 1000)
    // {
    //     util::timer timer;

    //     glb::optical.set_led_pwm(100);
    //     robot::chass.spin(70);
    //     // robot::intake.spin(127);
    //     pros::delay(300);
    //     double initColor = glb::optical.get_hue();
    //     glb::controller.print(1, 1, "%f", glb::optical.get_hue());
    //     // robot::chass.stop("b");
    //     bool initRed = initColor >= 60 ? false : true;

    //     double red = 10;
    //     double blue = 200;
    //     double speed = 100;

    //     if (!glb::red)
    //     {
    //         spinUntil(red, speed, timer, timeLimit);            
    //     }

    //     else 
    //     {
    //         spinUntil(blue, speed, timer, timeLimit);
    //     }

    //     glb::optical.set_led_pwm(0);
    //     robot::chass.stop("b");
    //     robot::intake.stop("b");
    // }

    void toggle(bool ym, double timeLimit = 1000)
    {
        util::timer timer;

        glb::optical.set_led_pwm(100);
        robot::chass.spin(45);
        pros::delay(400);
        double initColor = glb::optical.get_hue();
        glb::controller.print(1, 1, "%f", glb::optical.get_hue());
        // robot::chass.stop("b");
        bool initRed = initColor >= 60 ? false : true;

        double red = 10;
        double blue = 200;
        double speed = 110;

        if (!glb::red)
        {
            if(initRed)
            {
                spinUntil(blue, 127, timer, timeLimit);
                spinUntil(red, speed, timer, timeLimit);
            }

            else
            {
                spinUntil(red, speed, timer, timeLimit);
            }
            
        }
        else 
        {
            if(initRed)
            {
                spinUntil(blue, speed, timer, timeLimit);
            }

            else
            {
                spinUntil(red, speed, timer, timeLimit);
                spinUntil(blue, speed, timer, timeLimit);
            }
        }

        glb::optical.set_led_pwm(0);
        robot::chass.stop("b");
        robot::intake.stop("b");
    }

    // void toggle(bool half)
    // {
    //     robot::intake.spin(127); 

    //     if(half)
    //     {
    //         robot::chass.spin();
    //         pros::delay(230);
    //         robot::chass.stop(  "c");
    //     }

    //     else
    //     {
    //         robot::chass.spin();
    //         pros::delay(250);
    //         robot::chass.stop("c");
    //     }
    // }

}






