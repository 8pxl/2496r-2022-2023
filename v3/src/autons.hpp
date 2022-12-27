#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"
#include "global.hpp"

using namespace robot;

void wp() //NOLINT
{
    // chass.drive(100, 400, 10);
    // chass.spinTo(10, 4);
}

fptr WP = wp; //NOLINT 
std::vector<fptr> autons{WP}; //NOLINT
std::vector<std::string> autonNames{"wp"}; //NOLINT

#endif

