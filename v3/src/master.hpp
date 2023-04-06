#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include "cata.hpp"
#include "intake.hpp"


void keej()
{
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    bool L1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool NL1 = glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
    bool L2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool R1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool R2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool limit = glb::limit.get_value();
    std::cout << glb::limit.get_value() << std::endl;

    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;

    robot::chass.spinDiffy(lvolt,rvolt);

    if(R1)
    {
        robot::itsuki.spin(127);
    }

    else if(cata::curr == cata::idle)
    {
        robot::itsuki.stop('c');
        std::cout << "curr state: idle, stopping intake!" << std::endl;
    }
    
    if(NL1)
    {
        cata::fire();
    }

    if(R2)
    {
        cata::pause();
    }
}

void felix()
{
    double lStick = -glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    bool L1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool NL1 = glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
    bool L2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool R1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool R2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool limit = glb::limit.get_value();

    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;

    robot::chass.spinDiffy(lvolt,rvolt);

    if(R2)
    {
        robot::itsuki.spin(127);
    }
    
    else if(cata::curr == cata::idle)
    {
        robot::itsuki.stop('c');
        std::cout << "curr state: idle, stopping intake!" << std::endl;
    }
    
    if(NL1)
    {
        cata::fire();
    }
}