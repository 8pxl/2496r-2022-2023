#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"
#include "global.hpp"

using namespace robot;
using namespace lib;
using namespace util;

void wp() 
{
    pidConstants driveConstants(10,2,3,4,5,6);
    // stager wp
    // ({
    //     {&chassis::drive, {100,400}, {10, 1, pid(driveConstants, 10)}},
    //     {&chassis::drive, {500,800}, {20, 1, pid(driveConstants, 20)}}
    // });

    // wp.run(util::timer());
}
    
fptr WP = wp; 
std::vector<fptr> autons{WP}; 
std::vector<std::string> autonNames{"wp"};

#endif

