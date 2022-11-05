#ifndef __GLOBAL__
#define __GLOBAL__

#include "lib/lib.hpp"

namespace glb
{
    // motors
    pros::Motor frontLeft(1, pros::E_MOTOR_GEARSET_06, true); //NOLINT 
    pros::Motor midLeft(12, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backLeft(9, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor frontRight(4, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backRight(10, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor midRight(13, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor intake(14, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor puncher(20, pros::E_MOTOR_GEARSET_06, false); //NOLINT

    // sensors
    pros::Imu imu(5); //NOLINT
    pros::Controller controller(pros::E_CONTROLLER_MASTER);    //NOLINT
    pros::ADIEncoder leftEncoder(3,4,false); //NOLINT
    pros::ADIEncoder horizEncoder(1,2,false); //NOLINT
    pros::Optical optical(20); //NOLINT
    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
    util::coordinate pos = util::coordinate(0,0); //NOLINT
    bool red; //NOLINT
}

namespace robot
{
    lib::diffy chassisMotors = lib::diffy(std::vector<pros::Motor>{glb::frontLeft,glb::midLeft,glb::backLeft, glb::frontRight,glb::midRight,glb::backRight}); //NOLINT
    lib::imu imu = lib::imu(glb::imu,0); //NOLINT

    lib::chassis chass(chassisMotors, imu, glb::pos); //NOLINT
} 

#endif