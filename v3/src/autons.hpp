#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"
#include "global.hpp"

using namespace robot;
using namespace lib;
using namespace util;

#define btwn(a, b, c) if(time.time() > a && time.time() < b) {c
#define mv(c, d, e, f, g)  c(f + d, e);} else {int f = g;}
#define _ ;}

void wp() 
{
    timer time;
    pidConstants driveConstants(10,2,3,4,5,6);
    int first;
    
    while (true)
    {
        //default motor behavior
        chassisMotors.stop('c');
        itsuki.stop('c');
        if (!glb::limit.get_value())
        {
            robot::itsuki.spin(127);
        }

        //time based auton code
        btwn(100, 400, mv(chass.drive, 10, driveConstants, first, chassisMotors.getRotation()));
        btwn(300, 400, itsuki.spin(-127)_);
        
    
        pros::delay(10);
    }

    //your mom
    //ur mom
    //ur birther
}
    
fptr WP = wp; 
std::vector<fptr> autons{WP}; 
std::vector<std::string> autonNames{"wp"};

#undef btwn
#undef mv
#undef _
#endif

