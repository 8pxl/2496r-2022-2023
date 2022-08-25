#include "main.h"
#include "global.hpp"
#include "pros/misc.h"

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
        robot::intake.stop("c");
    }

    // fw

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
        flywheel::target = 600;
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        flywheel::target = 400;
    }

    flywheel::target -= 1;
}