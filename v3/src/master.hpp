#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include "cata.hpp"
#include "intake.hpp"

enum buttons
{
    L1 = 0,
    NL1 = 1,
    L2 = 2,
    NL2 = 3,
    R1 = 4,
    NR1 = 5,
    R2 = 6,
    NR2 = 7,
    UP = 8,
    NUP = 9,
    DOWN = 10,
    NDOWN = 11,
    LEFT = 12,
    NLEFT = 13,
    RIGHT = 14,
    NRIGHT = 15,
    X = 16,
    NX = 17,
    B = 18,
    NB = 19,
    Y = 20,
    NY = 21,
    A = 22,
    NA = 23 
};

void keej()
{
    std::vector<bool> cont = robot::controller.getAll(ALLBUTTONS);
    robot::chass.spinDiffy(robot::controller.drive(1, util::controller::arcade));
    
    if(cont[NL1]) cata::fire();
    if(cont[NR2]) robot::tsukasa.toggle();
    if(cont[NR1] && robot::tsukasa.getState()) robot::tsukasa.toggle();

    if(cont[R1]) robot::itsuki.spin(127);
    else if(cata::curr == cata::idle) robot::itsuki.stop('c');
}


// double lStick = glb::controller.getbe_analog(ANALOG_LEFT_Y);
// double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
// double rvolt = lStick - rStick;
// double lvolt = lStick + rStick;

void felix()
{
    std::vector<bool> cont = robot::controller.getAll(ALLBUTTONS);
    double lStick = -glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
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

    if(cont[NR2] && robot::tsukasa.getState())
    {
        robot::tsukasa.toggle();
    }
}
