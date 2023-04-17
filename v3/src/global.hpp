#ifndef __GLOBAL__
#define __GLOBAL__

#include "lib/lib.hpp"
#include <numeric>

namespace glb
{
    // motors
    pros::Motor frontLeft(7, pros::E_MOTOR_GEARSET_06, true); //NOLINT 
    pros::Motor midLeft(8, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backLeft(9, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor frontRight(4, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor backRight(2, pros::E_MOTOR_GEARSET_06, false); //NOLINT
    pros::Motor midRight(3, pros::E_MOTOR_GEARSET_06, true); //NOLINT
    pros::Motor yuuta(1, pros::E_MOTOR_GEARSET_18, false); //NOLINT
    pros::Motor saki(10, pros::E_MOTOR_GEARSET_18, true); //NOLINT

    // pistons
    pros::ADIDigitalOut boost('B'); //NOLINT
    // pros::ADIDigitalOut boostTwo('H'); //NOLINT
    // pros::ADIDigitalOut passOne('B'); //NOLINT
    // pros::ADIDigitalOut ptoOne('B'); //NOLINT
    pros::ADIDigitalOut derrick('C'); 


    // sensors
    pros::Imu imu(5); //NOLINT
    pros::Controller controller(pros::E_CONTROLLER_MASTER);    //NOLINT
    pros::ADIDigitalIn limit(1); //NOLINT
    // pros::ADIEncoder leftEncoder(3,4,false); //NOLINT
    // pros::ADIEncoder horizEncoder(1,2,false); //NOLINT
    pros::Optical optical(20); //NOLINT

    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
 //NOLINT
    bool red; //NOLINT
    util::coordinate pos(0,0);
}

namespace robot
{
    //motors
    lib::diffy chass(std::vector<pros::Motor>{glb::frontLeft,glb::midLeft,glb::backLeft, glb::frontRight,glb::midRight,glb::backRight}); 
    // lib::diffy chassisMotors(std::vector<pros::Motor>{glb::frontRight,glb::midRight,glb::backRight, glb::frontRight,glb::midRight,glb::backRight}); //NOLINT
    lib::diffy itsuki(std::vector<pros::Motor> {glb::yuuta, glb::saki} ); //NOLINT

    //pistons
    lib::pis boost({glb::boost}, false, ""); //NOLINT
    // // lib::pis boostTwo({glb::boostOne}, true, ""); //NOLINT
    // lib::pis pass({glb::passOne}, true, ""); //NOLINT
    // // lib::pis passTwo({glb::passTwo}, true, ""); //NOLINT
    // lib::pis pto({glb::ptoOne, glb::ptoTwo}, true, ""); //NOLINT
    lib::pis tsukasa({glb::derrick}, false, "");

    //sensors
    lib::imu imu(glb::imu,0); //NOLINT
    util::controller controller(glb::controller);
    // lib::limit limit(glb::limit); //NOLINT

    //subsytem objects
    // lib::chassis chass(chassisMotors, imu, glb::pos, glb::DL, glb::DR); //NOLINT
} 

#endif