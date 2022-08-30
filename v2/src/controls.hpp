#include "main.h"
#include "global.hpp"
#include "chassis.hpp"
#include "pros/misc.h"
#include "util.hpp"

util::timer decelTimer; 
void driveContol()
{
    // chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

    // intake
    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
        robot::intake.spin(127);
    }

    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        robot::intake.spin(-127);
    }

    else
    {
        // rollers::spin();
        robot::intake.stop("c");
    }

    // fw

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 500;
        }

        else
        {
            flywheel::target = 600;
        }

        decelTimer.start();
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 300;
        }

        else
        {
            flywheel::target = 400;
        }
        decelTimer.start();
    }

    //aut

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
    {
        chas::spinTo(90, 2000, 1);
        chas::drive(1200, 3000, 20);
        // chas::moveTo(util::coordinate(0,0), 100000, 0.6, 0.4, 1);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        double target = absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        target = target >= 0 ? target : 180 + (180 - fabs(target)); //conver to 0-360
        glb::controller.print(0, 0, "%f", target);

        // chas::spinTo(135, 1000, 1);
        chas::spinTo(target, 5000, 1);
    }
    

    // glb::controller.print(0,0, "%f", glb::imu.get_heading());
    util::coordinate a = util::coordinate(0,0);
    // chas::moveToVel(a, 0, 0,  0);

    if (decelTimer.time() > 3000)
    {
        flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target = 300;
    }
}