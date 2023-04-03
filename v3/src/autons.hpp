#ifndef __AUTONS__
#define __AUTONS__

#include "global.hpp"
#include "cata.hpp"

using namespace robot;
using namespace lib;
using namespace util;

void wp() 
{
    cata::fire();
}
    
fptr WP = wp; 
std::vector<fptr> autons{WP}; 
std::vector<std::string> autonNames{"wp"};

#endif
