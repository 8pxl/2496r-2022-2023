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
    // pros::delay(100);
    // chass.spin(50);
    // pros::delay(100);
    // chass.stop('b');
    // pros::delay(100);
    // chass.spin(-50);
    // pros::delay(150);
    // chass.stop('b');
    // pros::delay(100);
    cata::curr = cata::reloading;
    cata::slowTimer.startTime = pros::millis() - 500;
    while(!(cata::curr == cata::idle))
    {
        pros::delay(10);
    }
    cata::boost = true;
    intake::spin(127);
    robot::tsukasa.toggle();
    pros::delay(300);
    profiledDrive(-7,150);
    profiledDrive(6,200);
    spinTo(355.5, 900, onethreeefive);
    cata::fire();
    pros::delay(600);
    spinTo(234, 900, onethreeefive);
    tsukasa.toggle();
    profiledDrive(24.3,50);
    tsukasa.toggle();
    pros::delay(600);
    // intake::asyncPiston(299);
    profiledDrive(21, 250);
    // tsukasa.toggle();
    spinTo(332,800,ninety);
    profiledDrive(14, 150);
    cata::fire();
    pros::delay(300);
    profiledDrive(-19,130);
    spinTo(221, 730, ninety);
    profiledDrive(62.7, 200);
    spinTo(287, 900, fourfive);
    cata::fire();
    pros::delay(300);
    // release.toggle();
    profiledDrive(-9,200);
    spinTo(227,400,fourfive);
    // cata::curr = cata::reloading;
    arcTurn(dtr(270), 435, 570, -1, arc);
    profiledDrive(4,30);
    profiledDrive(-4,200);

}

void elevenwp() 
{
    //s = r*theta
    //r = s/theta

    // intake::asyncAction(intake::intaking, 400, 127);
    // intake::asyncAction(intake::idling, 700, 127);
    // pros::delay(100);
    // chass.spin(50);
    // pros::delay(100);
    // chass.stop('b');
    // pros::delay(100);
    // chass.spin(-50);
    // pros::delay(150);
    // chass.stop('b');
    // pros::delay(100);
    cata::curr = cata::reloading;
    while(!(cata::curr == cata::idle))
    {
        pros::delay(10);
    }
    cata::boost = true;
    intake::spin(127);
    robot::tsukasa.toggle();
    pros::delay(300);
    profiledDrive(-7,60);
    profiledDrive(4.5,100);
    spinTo(358, 900, onethreeefive);
    cata::fire();
    pros::delay(200);
    spinTo(236, 900, onethreeefive);
    intake::spin(127);
    profiledDrive(15.8,100);
    spinTo(135,600,ninety);
    tsukasa.toggle();
    profiledDrive(5.6,100);
    tsukasa.toggle();
    pros::delay(650);
    profiledDrive(-9.3,100);
    spinTo(347,1100,oneeighty);
    cata::fire();
    pros::delay(200);
    // profiledDrive(-3,150);
    spinTo(225, 650, ninety);
    tsukasa.toggle();
    profiledDrive(14.8,0);
    tsukasa.toggle();
    pros::delay(200); 
    // intake::asyncPiston(299);
    profiledDrive(24.3, 200);
    // tsukasa.toggle();
    spinTo(328,800,ninety);
    profiledDrive(8,130);
    cata::fire();
    pros::delay(400);
    profiledDrive(-8,130);
    spinTo(224, 730, ninety);
    profiledDrive(55, 200);
    spinTo(289, 800, fourfive);
    cata::fire();
    pros::delay(200);
    spinTo(251,500,fourfive);
    chass.spin(70);
    pros::delay(420);
    chass.stop('b');
    pros::delay(200);
    chass.spin(-50);
    pros::delay(200);
    chass.stop('b');
    pros::delay(100);
}

// - old shoots two
// void wp() 
// {
//     //s = r*theta
//     //r = s/theta

//     // intake::asyncAction(intake::intaking, 400, 127);
//     // intake::asyncAction(intake::idling, 700, 127);
//     cata::boost = false;
//     cata::curr = cata::reloading;
//     // pros::delay(100);
//     // chass.spin(50);
//     // pros::delay(100);
//     // chass.stop('b');
//     // pros::delay(100);
//     // chass.spin(-50);
//     // pros::delay(150);
//     // chass.stop('b');
//     // pros::delay(100);

//     chass.spin(30);
//     pros::delay(100);
//     chass.stop('b');
//     pros::delay(100);
//     chass.spin(-30);
//     cata::boost = true;
//     pros::delay(200);
//     chass.stop('b');
//     pros::delay(200);
//     cata::fire();
//     pros::delay(300);
    // spinTo(225, 900, onethreeefive);
    // intake::spin(127);
    // profiledDrive(13.9,150);
    // spinTo(135,700,ninety);
    // tsukasa.toggle();
    // profiledDrive(6.3,100);
    // tsukasa.toggle();
    // pros::delay(650);
    // profiledDrive(-6.5,100);
    // spinTo(347,1100,oneeighty);
    // cata::fire();
//     pros::delay(300);
//     profiledDrive(-3,150);
//     spinTo(229, 800, ninety);
//     tsukasa.toggle();
//     profiledDrive(7.8,0);
//     tsukasa.toggle();
//     pros::delay(400);
//     // intake::asyncPiston(299);
//     profiledDrive(24.3, 200);
//     // tsukasa.toggle();
//     spinTo(330,800,ninety);
//     profiledDrive(6,130);
//     cata::fire();
//     pros::delay(400);
//     profiledDrive(-8,130);
//     spinTo(224, 730, ninety);
//     profiledDrive(55, 200);
//     spinTo(289, 800, fourfive);
//     cata::fire();
//     pros::delay(200);
//     spinTo(251,500,fourfive);
//     chass.spin(70);
//     pros::delay(420);
//     chass.stop('b');
//     pros::delay(200);
//     chass.spin(-50);
//     pros::delay(200);
//     chass.stop('b');
//     pros::delay(100);
// }

void eightwp()
{
    //s = r*theta
    //r = s/theta

    // intake::asyncAction(intake::intaking, 400, 127);
    // intake::asyncAction(intake::idling, 700, 127);
    cata::boost = false;
    cata::curr = cata::reloading;
    // pros::delay(100);
    // chass.spin(50);
    // pros::delay(100);
    // chass.stop('b');
    // pros::delay(100);
    // chass.spin(-50);
    // pros::delay(150);
    // chass.stop('b');
    // pros::delay(100);

    chass.spin(30);
    pros::delay(100);
    chass.stop('b');
    pros::delay(100);
    chass.spin(-30);
    cata::boost = true;
    pros::delay(200);
    chass.stop('b');
    pros::delay(200);
    cata::fire();
    pros::delay(300);
    spinTo(225, 900, onethreeefive);
    intake::spin(127);
    profiledDrive(13.9,150);
    spinTo(135,700,ninety);
    tsukasa.toggle();
    profiledDrive(7.2,100);
    tsukasa.toggle();
    pros::delay(650);
    profiledDrive(-6.5,100);
    spinTo(347,1100,oneeighty);
    cata::fire();
    pros::delay(300);
    profiledDrive(-3,150);
    spinTo(229, 800, ninety);
    tsukasa.toggle();
    profiledDrive(8.6,0);
    tsukasa.toggle();
    pros::delay(400);
    // intake::asyncPiston(299);
    profiledDrive(24.3, 200);
    // tsukasa.toggle();
    spinTo(330,800,ninety);
    profiledDrive(6,130);
    cata::fire();
    pros::delay(400);
    profiledDrive(-8,130);
    spinTo(221, 730, ninety);
    intake::stop();
    profiledDrive(61, 200);
    spinTo(270,500,fourfive);
    release.toggle();
    cata::fire();
    pros::delay(200);
    chass.spin(70);
    pros::delay(420);
    chass.stop('b');
    pros::delay(200);
    chass.spin(-50);
    pros::delay(200);
    chass.stop('b');
    pros::delay(100);
}

void nearHalf()
{
    //s = r*theta
    //r = s/theta

    // intake::asyncAction(intake::intaking, 400, 127);
    // intake::asyncAction(intake::idling, 700, 127);
    cata::boost = false;
    cata::curr = cata::reloading;
    // pros::delay(100);
    // chass.spin(50);
    // pros::delay(100);
    // chass.stop('b');
    // pros::delay(100);
    // chass.spin(-50);
    // pros::delay(150);
    // chass.stop('b');
    // pros::delay(100);

    chass.spin(30);
    pros::delay(100);
    chass.stop('b');
    pros::delay(100);
    chass.spin(-30);
    cata::boost = true;
    pros::delay(200);
    chass.stop('b');
    pros::delay(400);
    cata::fire();
    pros::delay(300);
    spinTo(225, 900, onethreeefive);
    intake::spin(127);
    profiledDrive(14.4,150);
    spinTo(135,800,ninety);
    tsukasa.toggle();
    profiledDrive(6.3,100);
    tsukasa.toggle();
    pros::delay(650);
    profiledDrive(-6.5,100);
    spinTo(347,1100,oneeighty);
    cata::fire();
    pros::delay(300);
    profiledDrive(3,100);
    spinTo(229, 900, ninety);
    tsukasa.toggle();
    profiledDrive(11);
    tsukasa.toggle();
    pros::delay(650);
    // intake::asyncPiston(299);
    profiledDrive(29, 200);
    // tsukasa.toggle();
    spinTo(327,800,ninety);
    // profiledDrive(6,130);
    cata::fire();
    pros::delay(1300);
    arcTurn(dtr(354), 340, 800, -1, arc);
    profiledDrive(18, 100);
    spinTo(337, 900, fourfive);
    profiledDrive(-20, 200);
    cata::fire();
}

void skills()
{
    intake::spin(127);
    profiledDrive(-10, 100);
    profiledDrive(15, 100);
    spinTo(270, 600, ninety);
    profiledDrive(14, 100);
    profiledDrive(-7, 100);
    spinTo(178, 600, ninety);
    profiledDrive(-45, 100);
    cata::fire();
    pros::delay(200);
    profiledDrive(50, 100);
    spinTo(225, 600, fourfive);
    expansion.toggle();
}

void near()
{
    cata::curr = cata::reloading;
    cata::slowTimer.startTime = pros::millis() - 700;
    while(!(cata::curr == cata::idle))
    {
        pros::delay(10);
    }
    cata::boost = true;
    itsuki.spin(127);
    robot::tsukasa.toggle();
    pros::delay(300);
    profiledDrive(-7,150);
    profiledDrive(6,200);
    spinTo(355, 900, onethreeefive);
    itsuki.stop('c');
    cata::fire();
    pros::delay(700);
    spinTo(243, 900, onethreeefive);
    profiledDrive(17.4,150);
    itsuki.spin(127);
    spinTo(135,800,ninety);
    tsukasa.toggle();
    profiledDrive(7,100);
    tsukasa.toggle();
    pros::delay(650);
    profiledDrive(-6.5,100);
    spinTo(347,1100,oneeighty);
    itsuki.stop('c');
    cata::fire();
    pros::delay(700);
    profiledDrive(-4,100);
    spinTo(229, 900, ninety);
    tsukasa.toggle();
    profiledDrive(11);
    itsuki.spin(127);
    tsukasa.toggle();
    pros::delay(650);
    // intake::asyncPiston(299);
    profiledDrive(29, 200);
    // tsukasa.toggle();
    spinTo(327,800,ninety);
    profiledDrive(6,130);
    itsuki.stop('c');
    cata::fire();
    pros::delay(100);
    release.toggle();
}

void wpTruncate()
{
    cata::curr = cata::reloading;
    cata::slowTimer.startTime = pros::millis() - 500;
    while(!(cata::curr == cata::idle))
    {
        pros::delay(10);
    }
    cata::boost = true;
    intake::spin(127);
    robot::tsukasa.toggle();
    pros::delay(300);
    profiledDrive(-7,150);
    profiledDrive(6,200);
    spinTo(357.5, 900, onethreeefive);
    cata::fire();
    pros::delay(400);
    spinTo(234, 900, onethreeefive);
    tsukasa.toggle();
    profiledDrive(24.3,50);
    tsukasa.toggle();
    pros::delay(600);
    // intake::asyncPiston(299);
    profiledDrive(21, 200);
    // tsukasa.toggle();
    spinTo(332,800,ninety);
    profiledDrive(14, 200);
    cata::fire();
    pros::delay(300);
    release.toggle();
}
void far()
{
    cata::boost = true;
    cata::curr = cata::reloading;
    cata::slowTimer.startTime = pros::millis() - 500;
    while(!(cata::curr == cata::idle))
    {
        pros::delay(10);
    }
    tsukasa.toggle();
    profiledDrive(21, 100);
    itsuki.spin(127);
    tsukasa.toggle();
    pros::delay(450);
    profiledDrive(-5, 100);
    spinTo(17, 1200, ninety);
    itsuki.stop('c');
    cata::fire();
    pros::delay(250);
    cata::curr = cata::idle;
    spinTo(315, 900, fourfive);
    arcTurn(0.00001, 400, 900, -1, arc);
    cata::curr = cata::reloading;
    profiledDrive(5, 50);
    profiledDrive(-3, 50);
    spinTo(135, 900, onethreeefive);
    itsuki.spin(127);
    profiledDrive(50, 300);
    spinTo(55, 900, ninety);
    itsuki.stop('c');
    cata::fire();
    pros::delay(250);
    spinTo(0, 900 , fourfive);
    itsuki.spin(127);
    profiledDrive(20, 100);
    spinTo(5,900, fourfive);
    profiledDrive(25, 100);
    spinTo(0,900,fourfive);
    profiledDrive(-45, 200);
    spinTo(55, 900, ninety);
    itsuki.stop('c');
    cata::fire();
    release.toggle();
}

fptr WP = wp; fptr EIGHTWP = eightwp; fptr NEAR = near; fptr FAR = far; fptr WPTRUNCATE = wpTruncate; fptr SKILLS = skills;
std::vector<fptr> autons{WP, EIGHTWP, NEAR, FAR, WPTRUNCATE, SKILLS}; 
std::vector<std::string> autonNames{"wp", "eightwp", "near", "far", "truncated wp", "skills"};

#endif
