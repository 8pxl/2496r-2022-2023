#include "chassis.hpp"
#include "global.hpp"
#include "intake.hpp"
#include "flywheel.hpp"

void wp()
{
    flywheel::target = 480;

    //toggle roller
    robot::chass.spin(80);
    robot::intake.spin(110);
    pros::delay(450);
    robot::intake.stop("c");

    //drive and aim
    chas::drive(-500, 1000, 1);
    chas::spinTo(356, 800, 0.5);

    //shoot discs
    intake::index(1);
    pros::delay(1100);
    intake::index(2);

    //turn to 3 stack
    chas::spinTo(233, 1400, 1);
    robot::intake.spin(127); 
    
    //intake 3 stack
    robot::tsukasa.toggle();
    chas::drive(1300, 900, 20);
    robot::tsukasa.toggle();
    pros::delay(500);

    //aim and shoot discs
    flywheel::target = 420;
    chas::spinTo(345, 1300, 1);
    robot::intake.stop("c");
    flywheel::target = 480;
    intake::index(1);
    pros::delay(500);
    intake::index(1);
    pros::delay(500);
    intake::index(1);

    //allign with discs
    chas::drive(500, 600, 1);
    chas::spinTo(217, 1400, 1);

    //intake discs
    robot::intake.spin(127); 
    chas::drive(6500, 2500, 20);

    //toggle roller
    chas::spinTo(270, 800, 1);
    robot::intake.stop("b"); 
    robot::chass.spin(80);
    robot::intake.spin(110);
    pros::delay(300);
    robot::intake.stop("c");
}

void a(){}

// std::vector<void (*)()> autons{wp,a};
std::vector<void (*)()> autons{wp,a};
std::vector<std::string> autonNames{"wp","a"};
