#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"
#include "global.hpp"

typedef void(*fptr)();

using namespace robot;

void wp()
{
    chass.drive(1, 1, 1);
    chass.spinTo(10, 4);
}

fptr WP = wp; 

std::vector<fptr> autons{WP};
std::vector<std::string> autonNames{"wp"};

#endif