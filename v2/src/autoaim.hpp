#ifndef __AUTOAIM__
#define __AUTOAIM__

#include "global.hpp"  

void autoAim(int timeout, int color = 2, int center = 0, util::pidConstants constants = util::pidConstants(0.6, 0.2, 0, 0.1, 0.3, 1000))
{
    util::timer timer;
    util::pid pid(constants, 0);
    int CENTER;
    if (color == 1)
    {
        CENTER = 83;
    }
    else
    {
        CENTER = 68;
    }
    
    if (center)
    {
        CENTER = center;
    }
    int x;
    int vel;
    while (timer.time() < timeout)
    {
        pros::vision_object_s_t goal = glb::vision.get_by_sig(0, color);
        x = goal.left_coord;
        vel = pid.out(CENTER-x);
        robot::chass.spinDiffy(-vel, vel);
    }
    robot::chass.stop("b");
}
#endif