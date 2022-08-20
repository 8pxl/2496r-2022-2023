#include "main.h"
#include "global.hpp"

void driveContol()
{
    //chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

    //intake

}