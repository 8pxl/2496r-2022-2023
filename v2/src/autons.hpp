#include "chassis.hpp"
#include "global.hpp"
#include "intake.hpp"
#include "flywheel.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"

typedef void(*fptr)();
typedef void(&fref)();

void wp()
{
    glb::red = false;
    flywheel::target = 480;
    intake::toggle();

    //drive and aim
    chas::drive(-500, 1000, 1);
    chas::spinTo(356, 800);

    //shoot discs
    intake::index(1);
    pros::delay(1100);
    intake::index(2);

    //turn to 3 stack
    chas::spinTo(233, 1400);
    robot::intake.spin(127); 
    
    //intake 3 stack
    robot::tsukasa.toggle();
    chas::drive(1300, 900, 20);
    robot::tsukasa.toggle();
    pros::delay(500);

    //aim and shoot discs
    flywheel::target = 420;
    chas::spinTo(345, 1300);
    robot::intake.stop("c");
    flywheel::target = 480;
    intake::index(1);
    pros::delay(500);
    intake::index(1);
    pros::delay(500);
    intake::index(1);

    //allign with discs
    chas::drive(500, 600, 1);
    chas::spinTo(217, 1400);

    //intake discs
    robot::intake.spin(127); 
    chas::drive(6500, 2500, 20);

    //toggle roller
    chas::spinTo(270, 800);
    intake::toggle();
}

//Gerald was here
//asdfghjklkjhgfdsasdfghjklkjhgfdsawertyuikmnbvcxsaqw34567uiop;l,mnbvcxzsdfghj
//KJKJKJ

void skills()
{
    // robot::imu.init(180);
    util::pidConstants smallTurn = util::pidConstants(6, 1.6, 2, 0.05, 7, 10);
    util::pidConstants bigTurn = util::pidConstants(3.7, 1.5, 35, 0.05, 2.4, 20);
    util::pidConstants medTurn = util::pidConstants(4, 1.5, 20, 0.05, 2.4, 20);

    // chas::drive(2000, 3000, 1);
    // chas::spinTo(135, 3000,medTurn);
    // util::pidConstants linearConstants(0.3,0,0,0,0,0);
    // util::pidConstants rotationConstants(0,0,0,0,0,0);
    // chas::spinTo(90, 3000);
    // chas::drive(2000, 3000, 1);

    glb::red = true;
    flywheel::target = 350;

    // pros::delay(1000);


    for (int i = 0; i < 9; i++)
    {
        robot::intake.spin(127);
        pros::delay(800);
        robot::intake.stop("c");
        pros::delay(100);
        robot::intake.spin(-50);
        pros::delay(250);
        robot::intake.stop("c");
    }

    // chas::moveTo(util::coordinate(0,200), 60000, linearConstants, rotationConstants, 0, 0, 0);

    flywheel::target = 300;
    robot::intake.stop("c");
    chas::drive(2150, 1200, 1);

    chas::spinTo(273, 750);
    // chas::drive(400,600,5);
    intake::toggle();
    chas::drive(-300,750,5);
    chas::spinTo(90, 1000);
    robot::intake.spin(127);
    robot::tsukasa.toggle();
    chas::drive(750,800,5);
    robot::tsukasa.toggle();
    // pros::delay(200);
    chas::drive(325,500,5);
    chas::spinTo(0, 650);
    chas::drive(1600,800,5);
    intake::toggle();
    robot::intake.stop("c");
    chas::drive(-300,500,5);
    chas::spinTo(274, 650);
    flywheel::target = 350;
    chas::drive(-2700,1500,1);
    chas::spinTo(283, 1000,smallTurn);
    robot::intake.spin(-100);
    pros::delay(2000);
    robot::intake.stop("c");
    chas::spinTo(165, 800);
    robot::intake.spin(127);
    chas::drive(1000, 725, 5);
    chas::spinTo(180, 500, smallTurn);
    chas::drive(1000, 800, 5);
    chas::spinTo(240, 1000);
    robot::intake.spin(-100);
    pros::delay(2000);
    robot::intake.stop("c");
    chas::spinTo(0, 1500,medTurn);
    chas::drive(-3100, 2000, 5);
    chas::spinTo(90, 1000);
    robot::tsukasa.toggle();
    robot::intake.spin(127);
    robot::chass.spin(70);
    pros::delay(800);
    robot::tsukasa.toggle();
    robot::chass.spin(127);
    pros::delay(400);
    robot::chass.spin(70);
    pros::delay(300);
    intake::toggle();
    chas::drive(-1200,1000,5);
    chas::spinTo(180, 1000);
    robot::tsukasa.toggle();
    robot::chass.spin(127);
    pros::delay(500);
    intake::toggle();
    chas::spinTo(205, 1330);
    chas::drive(-3500, 1700, 1);
    robot::chass.spin(-20);
    pros::delay(3000);
    chas::spinTo(175, 1000, smallTurn);
    flywheel::target = 350;
    for (int i = 0; i < 10; i++)
    {
        robot::intake.spin(127);
        pros::delay(800);
        robot::intake.stop("c");
        pros::delay(100);
        robot::intake.spin(-50);
        pros::delay(250);
        robot::intake.stop("c");
    }

}

void nearHalf()
{

    util::pidConstants smallTurn = util::pidConstants(6, 1.6, 2, 0.05, 7, 10);
    util::pidConstants bigTurn = util::pidConstants(3.7, 1.5, 35, 0.05, 2.4, 20);
    util::pidConstants medTurn = util::pidConstants(4, 1.5, 20, 0.05, 2.4, 20);

    flywheel::target = 460;

    glb::red = false;

    //toggle roller
    intake::toggle();

    //drive and aim
    chas::drive(-500, 1000, 1);
    chas::spinTo(356, 800,smallTurn);

    //shoot discs
    intake::index(1);
    flywheel::target = 470;
    pros::delay(500);
    intake::index(2);
    flywheel::target = 445;

    //turn to 3 stack
    chas::spinTo(233, 1000);
    robot::intake.spin(127); 
    
    //intake 3 stack
    robot::tsukasa.toggle();
    chas::drive(1300, 900, 5);
    robot::tsukasa.toggle();
    pros::delay(500);

    //aim and shoot discs

    chas::drive(-450,600,5);
    chas::spinTo(350, 1100);
    robot::intake.stop("c");
    intake::index(1);
    flywheel::target = 460;
    pros::delay(500);
    intake::index(1);
    pros::delay(500);
    intake::index(1);
    flywheel::target = 440;

    chas::spinTo(135, 700);
    robot::tsukasa.toggle();
    robot::intake.spin(127);
    chas::drive(400,880,5);
    robot::tsukasa.toggle();
    pros::delay(400);
    chas::drive(-600,880,5);
    chas::spinTo(344, 1200,bigTurn);
    robot::intake.stop("B");
    intake::index(1);
    flywheel::target = 470;
    pros::delay(500);
    intake::index(1);
    pros::delay(500);
    intake::index(1);
}

void farHalf()
{
    util::pidConstants smallTurn = util::pidConstants(6, 1.6, 2, 0.05, 7, 20);

    // flywheel::target = 500;
    // chas::spinTo(35, 1300);
    // intake::index(1);
    // pros::delay(500);
    // intake::index(1);

    // flywheel::target = 300;
    // chas::spinTo(225, 1000);
    // robot::tsukasa.toggle();
    // chas::drive(1400, 1000, 5);
    // robot::intake.spin(127);
    // robot::tsukasa.toggle();
    // pros::delay(300);
    // flywheel::target = 500;
    // chas::drive(-200, 1000, 5);
    // chas::spinTo(15, 1000);
    // intake::index(1);
    // pros::delay(500);
    // intake::index(1);

    // chas::spinTo(350, 1000);
    // intake::toggle();
    
    robot::tsukasa.toggle();
    flywheel::target = 440;
    chas::drive(1200, 750, 5);
    robot::intake.spin(127);
    robot::tsukasa.toggle();
    pros::delay(600);
    chas::drive(-400, 800, 5);
    chas::spinTo(25, 1500);
    intake::index(1);
    flywheel::target = 470;
    // flywheel::target = 480;
    pros::delay(500);
    intake::index(1);
    pros::delay(500);
    intake::index(1);
    flywheel::target = 440;
    robot::intake.spin(127);
    chas::drive(700 , 1500, 5);
    chas::drive(-700, 1500, 5);
    intake::index(1);
    flywheel::target = 470;
    pros::delay(500);
    intake::index(1);
    chas::spinTo(325, 900);
    robot::chass.spin(127);
    pros::delay(1400);
    intake::toggle();


}

// std::vector<void (*)()> autons{wp,a};
fptr WP = wp; fptr SKILLS = skills; fptr NEARHALF = nearHalf; fptr FARHALF = farHalf;

std::vector<fptr> autons{WP,SKILLS, NEARHALF, FARHALF};
std::vector<std::string> autonNames{"wp","skills","nearHalf", "farHalf"};
