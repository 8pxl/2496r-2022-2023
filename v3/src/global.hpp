#ifndef __GLOBAL__
#define __GLOBAL__

#include "lib/lib.hpp"
#include <vector>

namespace glb
{
    // motors
    pros::Motor frontLeft(8, pros::E_MOTOR_GEARSET_06, true); //NOLINT 
    pros::Motor midLeft(9, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backLeft(7, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor frontRight(2, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backRight(1, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor midRight(4, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor yuuta(3, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor saki(10, pros::E_MOTOR_GEARSET_06, true); //NOLINT

    // pistons
    pros::ADIDigitalOut boostOne('E'); //NOLINT
    pros::ADIDigitalOut boostTwo('H'); //NOLINT
    pros::ADIDigitalOut passOne('B'); //NOLINT
    pros::ADIDigitalOut ptoOne('B'); //NOLINT
    pros::ADIDigitalOut ptoTwo('B'); //NOLINT


    // sensors
    pros::Imu imu(5); //NOLINT
    pros::Controller controller(pros::E_CONTROLLER_MASTER);    //NOLINT
    pros::ADIDigitalIn limit('F'); //NOLINT
    pros::ADIEncoder leftEncoder(3,4,false); //NOLINT
    pros::ADIEncoder horizEncoder(1,2,false); //NOLINT
    pros::Optical optical(20); //NOLINT

    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
    util::coordinate pos = util::coordinate(0,0); //NOLINT
    bool red; //NOLINT
    double DL = 0.5;
    double DR = 0.5;
}

namespace robot
{
    std::vector<int> a(100);
    //motors
    lib::diffy chassisMotors(std::vector<pros::Motor>{glb::frontLeft,glb::midLeft,glb::backLeft, glb::frontRight,glb::midRight,glb::backRight}); 
    // lib::diffy chassisMotors(std::vector<pros::Motor>{glb::frontRight,glb::midRight,glb::backRight, glb::frontRight,glb::midRight,glb::backRight}); //NOLINT
    lib::diffy itsuki(std::vector<pros::Motor> {glb::yuuta, glb::saki} ); //NOLINT

    //pistons
    lib::pis boost({glb::boostOne, glb::boostTwo}, true, ""); //NOLINT
    // lib::pis boostTwo({glb::boostOne}, true, ""); //NOLINT
    lib::pis pass({glb::passOne}, true, ""); //NOLINT
    // lib::pis passTwo({glb::passTwo}, true, ""); //NOLINT
    lib::pis pto({glb::ptoOne, glb::ptoTwo}, true, ""); //NOLINT

    //sensors
    lib::imu imu(glb::imu,0); //NOLINT
    lib::limit limit(glb::limit); //NOLINT

    //subsytem objects
    lib::chassis chass(chassisMotors, imu, glb::pos, glb::DL, glb::DR); //NOLINT
} 

#endif