#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include "cata.hpp"
#include "intake.hpp"

using namespace robot;

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
    chass.spinDiffy(robot::controller.drive(1, util::controller::arcade));
    
    if(cont[NL1]) cata::fire();
    if(cont[NR2]) tsukasa.toggle();
    if(cont[NR1] && tsukasa.getState()) tsukasa.toggle();
    if(cont[R1]) itsuki.spin(127);
    else if(cata::curr == cata::idle) itsuki.stop('c');

    if(cont[A]) wp();
    if(cont[B]) cata::boost = !cata::boost;
}

void felix()
{
    std::vector<bool> cont = robot::controller.getAll(ALLBUTTONS);
    chass.spinDiffy(robot::controller.drive(-1, util::controller::arcade));
    
    if(cont[NL1]) cata::fire();
    if(cont[NR1]) tsukasa.toggle();
    if(cont[NR2] && tsukasa.getState()) tsukasa.toggle();
    if(cont[R2]) itsuki.spin(127);
    else if(cata::curr == cata::idle) itsuki.stop('c');
}
