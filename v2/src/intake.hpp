#include "global.hpp"

void index(int num)
{
    for (int i = 0; i < num; i++)
    {
        robot::intake.spin(-50);
        pros::delay(250);
        robot::intake.stop("b");
        pros::delay(100);
    }
}
