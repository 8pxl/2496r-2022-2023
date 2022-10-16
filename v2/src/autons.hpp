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

    util::pidConstants smallTurn = util::pidConstants(8, 1.6, 2, 0.05, 7, 10);
    util::pidConstants bigTurn = util::pidConstants(3.7, 1.5, 35, 0.05, 2.4, 20);
    util::pidConstants medTurn = util::pidConstants(4, 1.5, 20, 0.05, 2.4, 20);

    flywheel::target = 450;
    intake::toggle();

    //drive and aim
    chas::drive(-500, 800, 1);
    chas::spinTo(357.5, 800,smallTurn);

    //shoot discs
    intake::index(1);
    // flywheel::target = 480;
    pros::delay(300);
    intake::index(2);
    flywheel::target = 415; 

    //turn to 3 stack
    chas::spinTo(233, 1000,medTurn);
    robot::intake.spin(127); 
    
    //intake 3 stack
    robot::tsukasa.toggle();
    chas::drive(1300, 800, 5);
    robot::tsukasa.toggle();
    flywheel::target = 455;
    pros::delay(500);

    //aim and shoot discs
    chas::spinTo(345.8, 1100);
    robot::intake.stop("c");
    intake::index(1);
    // flywheel::target = 460;
    pros::delay(300);
    intake::index(1);
    // flywheel::target = 465;
    pros::delay(300);
    intake::index(1);

    //allign with discs
    chas::drive(500, 600, 1);
    chas::spinTo(218, 1000);

    //intake discs
    robot::intake.spin(127); 
    chas::drive(6500, 2300, 20);

    //toggle roller
    chas::spinTo(270, 700);
    intake::toggle();
}

//Gerald was here
//asdfghjklkjhgfdsasdfghjklkjhgfdsawertyuikmnbvcxsaqw34567uiop;l,mnbvcxzsdfghj
//KJKJKJ

void skills()
{
    // robot::imu.init(180);
    util::pidConstants smallTurn = util::pidConstants(6, 1.6, 2, 0.05, 7, 10);
    util::pidConstants bigTurn = util::pidConstants(6.3, 1.74, 40, 0.05, 2.7, 30);
    util::pidConstants medTurn = util::pidConstants(4, 1.5, 20, 0.05, 2.4, 20);
    
    // chas::spinTo(180, 3000, bigTurn);

    // chas::drive(2000, 3000, 1);
    // chas::spinTo(135, 3000,medTurn);
    // util::pidConstants linearConstants(0.3,0,0,0,0,0);
    // util::pidConstants rotationConstants(0,0,0,0,0,0);
    // chas::spinTo(90, 3000);
    // chas::drive(2000, 3000, 1);

    // glb::red = true;
    flywheel::target = 370;

    pros::delay(1000);


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


    flywheel::target = 300;
    robot::intake.stop("c");

    //toggler roller #1
    chas::drive(2320, 1200, 1);
    chas::spinTo(273, 750);
    // chas::drive(400,600,5);
    intake::toggle();

    //intake first three stack
    chas::drive(-300,750,5);
    chas::spinTo(90, 1000,bigTurn);
    robot::intake.spin(127);
    robot::tsukasa.toggle();
    chas::drive(750,800,5);
    robot::tsukasa.toggle();
    // pros::delay(200);
    chas::drive(325,500,5);

    //toggler roller #2
    chas::spinTo(0, 650);
    chas::drive(1600,800,5);
    intake::toggle();
    robot::intake.stop("c");

    //shoot first volley
    chas::drive(-300,500,5);
    chas::spinTo(274, 650);
    flywheel::target = 390;
    chas::drive(-2700,1500,1);
    chas::spinTo(293, 1000);
    robot::intake.spin(-50);
    pros::delay(1200);
    robot::intake.stop("c");

    //intake second round of three
    chas::spinTo(165, 800);
    robot::intake.spin(127);
    flywheel::target = 400;
    chas::drive(1000, 725, 5);
    chas::spinTo(180, 500, smallTurn);
    chas::drive(1400, 800, 5);

    //second volley of three
    // flywheel::target = 400;
    chas::spinTo(233, 800);
    robot::intake.stop("c");
    robot::tsukasa.toggle();
    robot::intake.spin(-25);
    // flywheel::target = 420;
    pros::delay(2000);
    robot::intake.stop("c");

    //intake third round of three and roller #3
    chas::spinTo(0, 1200,medTurn);
    robot::tsukasa.toggle();
    chas::drive(-2500, 1300, 5);
    chas::spinTo(90, 800);
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

    //roller #4
    chas::drive(-1200,850,5);
    chas::spinTo(180, 750);
    // robot::tsukasa.toggle();
    robot::chass.spin(100);
    pros::delay(500);
    intake::toggle();
    robot::tsukasa.toggle();

    //allign for matchloads
    robot::intake.stop("c");
    chas::spinTo(210, 1000);
    chas::drive(-3600, 1900, 1);
    robot::chass.spin(-20);
    pros::delay(2000);
    robot::chass.spinDiffy(-10, 40);
    pros::delay(630);
    robot::chass.stop("b");
    pros::delay(200);
    robot::chass.spinDiffy(-30, -30);
    pros::delay(200);
    robot::chass.stop("b");
    // chas::spinTo(172, 1000, smallTurn);
    flywheel::target = 370;
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

    chas::spinTo(190, 1000);
    chas::drive(3000, 1500, 5);
    chas::spinTo(135, 1000);
    chas::drive(-400,1500,5);
    robot::expansion.toggle();

}

void nearHalf()
{

    util::pidConstants smallTurn = util::pidConstants(6, 1.6, 2, 0.05, 7, 10);
    util::pidConstants bigTurn = util::pidConstants(3.7, 1.5, 35, 0.05, 2.4, 20);
    util::pidConstants medTurn = util::pidConstants(4, 1.5, 20, 0.05, 2.4, 20);

    flywheel::target = 445;
    intake::toggle();

    //drive and aim
    chas::drive(-500, 700, 1);
    chas::spinTo(357, 650,smallTurn);

    //shoot discs
    intake::index(1);
    flywheel::target = 600;
    pros::delay(90);
    intake::index(1);
    flywheel::target = 415; 

    //turn to 3 stack
    chas::spinTo(233, 1000,medTurn);
    robot::intake.spin(127); 
    
    //intake 3 stack
    robot::tsukasa.toggle();
    chas::drive(1200, 700, 5);
    robot::tsukasa.toggle();
    pros::delay(500);

    //aim and shoot discs
    flywheel::target = 415;
    chas::spinTo(347.5, 1100);
    robot::intake.stop("c");
    intake::index(1);
    flywheel::target = 600;
    pros::delay(65);
    intake::index(1);
    pros::delay(45);
    intake::index(1);
    flywheel::target = 465;
    // intake::waitIndex(410);
    flywheel::target = 420;

    chas::spinTo(222, 1000);
    chas::drive(-500,650,5);
    chas::spinTo(135, 700);
    robot::tsukasa.toggle();
    robot::intake.spin(127);
    chas::drive(300,650,5);
    robot::tsukasa.toggle();
    pros::delay(450);
    chas::drive(-300,800,5);
    chas::spinTo(347.5, 1200,bigTurn);
    robot::intake.stop("B");
    intake::index(1);
    flywheel::target = 600;
    pros::delay(65);
    intake::index(1);
    pros::delay(65);
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
