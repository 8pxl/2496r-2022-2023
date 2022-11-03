#ifndef __AUTONS__
#define __AUTONS__

#include "lib/lib.hpp"

typedef void(*fptr)();

void wp()
{

}

fptr WP = wp; 

std::vector<fptr> autons{WP};
std::vector<std::string> autonNames{"wp"};

#endif