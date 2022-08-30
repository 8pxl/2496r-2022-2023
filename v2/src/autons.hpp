#include "global.hpp"
#include <string>

void wp(){}

std::vector<std::function<void(void)>> autons{wp};
std::vector<std::string> autonNames{"wp"};
