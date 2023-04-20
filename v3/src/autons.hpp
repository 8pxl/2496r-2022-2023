#ifndef __AUTONS__
#define __AUTONS__

#include "global.hpp"
#include "chassis.hpp"
#include "cata.hpp"
#include "intake.hpp"

using namespace robot;
using namespace chas;
using namespace lib;
using namespace intake;
using namespace util;

//p,i,d,tolerance,integralThreshold, maxIntegral, kv
pidConstants fourfive(10, 0.3, 90, 0.05, 1.3, 20); 
pidConstants ninety(5.9, 0.6, 70, 0.05, 1.1, 20); 
pidConstants onethreeefive(6.3, 0.4, 70, 0.05, 1.4, 20); 
pidConstants oneeighty(6.4, 0.4, 90, 0.05, 1.2, 20); 

pidConstants arc(2.8,0.2,20,0.05,5,100);
// pidConstants arc(2.3,0,0,0.05,5,100);
// void wp() 
// {
//     // intake::asyncAction(intake::intaking, 400, 127);
//     // intake::asyncAction(intake::idling, 700, 127);
//     // chas::profiledDrive(70);
//     // pros::delay(3000);
//     cata::boost = true;
//     cata::fire();
//     chass.spin(50);
//     pros::delay(100);
//     chass.stop('b');
//     pros::delay(100);
//     chass.spin(-50);
//     pros::delay(100);
//     chass.stop('b');
//     pros::delay(100);
//     spinTo(225, 900, fourfive);
//     intake::spin(127);
//     profiledDrive(40, 200);
//     spinTo(340,800,ninety);
//     cata::fire();
//     pros::delay(300);
//     spinTo(225, 900, fourfive);
//     profiledDrive(-35,200);
//     spinTo(135,900,ninety);
//     profiledDrive(10,100);
//     profiledDrive(-10,100);
//     spinTo(340,1100,oneeighty);
//     cata::fire();
// }

void wp() 
{
    //s = r*theta
    //r = s/theta

    // intake::asyncAction(intake::intaking, 400, 127);
    // intake::asyncAction(intake::idling, 700, 127);
    cata::boost = true;
    cata::fire();
    // pros::delay(100);
    // chass.spin(50);
    // pros::delay(100);
    // chass.stop('b');
    // pros::delay(100);
    // chass.spin(-50);
    // pros::delay(150);
    // chass.stop('b');
    // pros::delay(100);

    pros::delay(100);
    chass.spin(50);
    pros::delay(200);
    chass.stop('b');
    pros::delay(100);
    chass.spin(-50);
    pros::delay(150);
    chass.stop('b');
    pros::delay(100);
    spinTo(225, 900, onethreeefive);
    intake::spin(127);
    profiledDrive(12,100);
    spinTo(135,800,ninety);
    tsukasa.toggle();
    profiledDrive(6.8,100);
    tsukasa.toggle();
    pros::delay(650);
    profiledDrive(-7,100);
    spinTo(347,1100,oneeighty);
    cata::fire();
    pros::delay(300);
    spinTo(225, 900, ninety);
    tsukasa.toggle();
    profiledDrive(8.6);
    tsukasa.toggle();
    pros::delay(650);
    // intake::asyncPiston(299);
    profiledDrive(29, 200);
    // tsukasa.toggle();
    spinTo(327,800,ninety);
    profiledDrive(12,130);
    cata::fire();
    pros::delay(1300);
    arcTurn(dtr(354), 340, 800, -1, arc);
    profiledDrive(27, 100);
    spinTo(337, 900, fourfive);
    profiledDrive(-26, 200);
    cata::fire();
}
    
fptr WP = wp; 
std::vector<fptr> autons{WP}; 
std::vector<std::string> autonNames{"wp"};

#endif
