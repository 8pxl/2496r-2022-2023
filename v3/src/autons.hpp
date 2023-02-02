#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"
#include "global.hpp"

using namespace robot;
using namespace lib;
using namespace util;

#define btwn(a, b, c) if(time.time() > a && time.time() < b) {c
#define mv(c, d, e, f, g)  c(f + d, e);} else {int f = g;}
#define sp(c) c;}

void wp() 
{
    timer time;
    pidConstants driveConstants(10,2,3,4,5,6);
    const pid one(driveConstants, 0);
    int first;
    
    while (true)
    {
        btwn(100, 400, mv(chass.drive, 10, one, first, chassisMotors.getRotation()));
        btwn(300, 400, sp(itsuki.spin(127)));
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
#undef sp
#endif

