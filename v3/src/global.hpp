#ifndef __GLOBAL__
#define __GLOBAL__

#include "lib/lib.hpp"

namespace glb
{
    // motors
    pros::Motor frontLeft(1, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor midLeft(12, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backLeft(9, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor frontRight(4, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backRight(10, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor midRight(13, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor intake(14, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor puncher(20, pros::E_MOTOR_GEARSET_06, false);

    //pistons
    pros::ADIDigitalOut derrick(6);
    pros::ADIDigitalOut cata(5);
    pros::ADIDigitalOut plane(7);

    // sensors
    pros::Imu imu(5);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);   
    pros::ADIEncoder leftEncoder(3,4,false);
    pros::ADIEncoder horizEncoder(1,2,false);
    pros::Optical optical(20);
    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
    util::coordinate pos = util::coordinate(0,0);
    util::timer matchTimer(1);
    // double dl;
    // double dr;
    bool red;
    bool match = false;
    bool driver;
}

namespace robot
{
    lib::diffy chassisMotors = lib::diffy(std::vector<pros::Motor>{glb::frontLeft,glb::midLeft,glb::backLeft, glb::frontRight,glb::midRight,glb::backRight});
    lib::imu imu = lib::imu(glb::imu,0);

    
    lib::chassis chass(chassisMotors, imu, glb::pos);
} 

#endif