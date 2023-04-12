#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include "cata.hpp"
#include "intake.hpp"

void keej()
{
    std::vector<bool> cont = robot::controller.getAll(ALLBUTTONS);
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    bool limit = glb::limit.get_value();
    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;

    robot::chass.spinDiffy(lvolt,rvolt);

    if(cont[R1])
    {
        robot::itsuki.spin(127);
    }

    else if(cata::curr == cata::idle)
    {
        robot::itsuki.stop('c');
    }
    
    if(cont[NL1])
    {
        cata::fire();
    }

    if(cont[NR2])
    {
        robot::tsukasa.toggle();
    }

    if(cont[NR1] && robot::tsukasa.state)
    {
        robot:tsukasa.toggle();
    }
}

void felix()
{
    double lStick = -glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    std::vector<bool> cont = robot::controller.getAll(ALLBUTTONS);
    bool limit = glb::limit.get_value();
    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;

    robot::chass.spinDiffy(lvolt,rvolt);

    if(cont[R2])
    {
        robot::itsuki.spin(127);
    }
    
    else if(cata::curr == cata::idle)
    {
        robot::itsuki.stop('c');
    }
    
    if(cont[NL1])
    {
        cata::fire();
    }

    if(cont[NR1])
    {
        robot::tsukasa.toggle();
    }

    if(cont[NR1] && robot::tsukasa.state)
    {
        robot::tsukasa.toggle();
    }
}