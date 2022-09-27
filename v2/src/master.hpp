#include "flywheel.hpp"
#include "main.h"
#include "global.hpp"
#include "chassis.hpp"
#include "pros/misc.h"
#include "util.hpp"
#include "autons.hpp"

util::timer decelTimer; 


void curvature(double iThrottle, double iCurvature, double iThreshold){
    if(std::fabs(iThrottle) <= iThreshold){
        robot::chass.spinDiffy(127*iCurvature,0-127*iCurvature);
        return;
    }

    iThrottle = std::fabs(iThrottle) > iThreshold ? iThrottle : 0;
    iCurvature = std::fabs(iCurvature) > iThreshold ? iCurvature : 0;

    double left = iThrottle + std::fabs(iThrottle) * iCurvature;
    double right = iThrottle - std::fabs(iThrottle) * iCurvature;

    double mag = std::max(std::fabs(left), std::fabs(right));
    if(mag > 1.0){
        left /= mag;
        right /= mag;
    }

    // leftSideMotor->moveVoltage(left * maxVoltage);
    // rightSideMotor->moveVoltage(right * maxVoltage);
    robot::chass.spinDiffy(left * 127,right*127);
}


void driveContol()
{
    // chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    // curvature(lStick/127, rStick/127, 0.1);
    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

    // robot::chass.spinDiffy(lStick, rStick);


    // fw


    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            flywheel::target = 360;
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            flywheel::target = 580;
        }
    
        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 430;
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            flywheel::target = 400;
        }

        decelTimer.start();
    }

    else
    {
        if(robot::tsukasa.state)
        {
            if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
            {
                robot::tsukasa.toggle();
            }
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            robot::intake.spin(127);
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            robot::intake.spin(-100);
            decelTimer.start();
            decelTimer.startTime += 3000;
        }

        else
        {
            robot::intake.stop("c");
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
        {
            robot::tsukasa.toggle();
        }
    }

    //aut

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
    {
        intake::toggle();
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
    {
        // chas::moveTo(util::coordinate(0,0), 100000, 0.7, 4, 0.6);

        glb::red = false;
        flywheel::target = 480;

        //toggle roller
        // robot::chass.spin(80);
        // robot::intake.spin(110);
        // pros::delay(450);
        // robot::intake.stop("c");
        intake::toggle();

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
        intake::toggle();
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        // double target = 180 - absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        // target = target >= 0 ? target : 180 + fabs(target); //conver to 0-360
        // glb::controller.print(0, 0, "%f", target);

        // // chas::spinTo(135, 1000, 1);
        // chas::spinTo(target, 5000, 1);
        robot::intake.spin(-127);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
    {
        glb::pos = util::coordinate(103,675);
        glb::red = false;
        robot::imu.init(180);

        util::pidConstants linearConstants(2.9,0,0,0,0);
        util::pidConstants rotationConstants(1.3,0,0,30,0);

        flywheel::target = 420;
    
        //toggle roller
        intake::toggle();

        chas::drive(-500, 1000, 1);
        chas::spinTo(176, 800, 0.5);

        intake::index(2);  
        flywheel::target = 200; 
        robot::tsukasa.toggle();
        chas::spinTo(53, 1000, 1);
        // chas::spinTo((util::absoluteAngleToPoint(glb::pos, util::coordinate(167,600))), 10000, 4);
        chas::drive(1300, 900, 10);
        robot::tsukasa.toggle();
        robot::intake.spin(127);
        pros::delay(300);
        flywheel::target = 420;
        // double target = util::absoluteAngleToPoint(glb::pos, util::coordinate(0,0)) - 180-4;
        chas::spinTo(165, 1300, 40);
        intake::index(3);

    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
    {
        flywheel::target = 440;

        glb::red = false;

        //toggle roller
        intake::toggle();

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

        flywheel::target = 440;
        chas::drive(-400,600,5);
        chas::spinTo(350, 1300, 1);
        robot::intake.stop("c");
        intake::index(1);
        pros::delay(500);
        intake::index(1);
        pros::delay(500);
        intake::index(1);

        chas::spinTo(135, 700, 3);
        robot::tsukasa.toggle();
        robot::intake.spin(127);
        chas::drive(600,1000,5);
        robot::tsukasa.toggle();
        flywheel::target = 460;
        pros::delay(300);
        chas::drive(-600,1000,5);
        chas::spinTo(351, 1200, 2);
        robot::intake.stop("B");
        intake::index(1);
        pros::delay(500);
        intake::index(1);
        pros::delay(500);
        intake::index(1);
    }

    
    // glb::controller.print(0,0, "%f", glb::imu.get_heading());
    // util::coordinate a = util::coordinate(0,0);
    // chas::moveToVel(a, 0, 0,  0);

    if (decelTimer.time() > 6000)
    {
        flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target != 0 ? flywheel::target = 300 : flywheel::target = 0;
    }
    glb::controller.print(0, 0, "%f", flywheel::target);
}

void (*autonSelector())()
{

    int numAutons = autons.size() - 1;
    int selectedAut = 0;
    bool autSelected = false;

    while(1)
    {   
        glb::controller.clear();

        if(robot::flywheel.getRotation() >= 60 || glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
        {
            selectedAut = selectedAut == numAutons ? 0 : selectedAut++;
            robot::flywheel.spin(-2);
            pros::delay(100);
            robot::flywheel.reset();
        }

        if(robot::flywheel.getRotation() <= -60 || glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            selectedAut = selectedAut == 0 ? numAutons : selectedAut --;
            robot::flywheel.spin(-2);
            pros::delay(100);
            robot::flywheel.reset();
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            break;
        }

        glb::controller.print(0, 0, "%s", autonNames[selectedAut]);

        // glb::controller.print(0, 0, "%f", robot::flywheel.getRotation());

    }

    return(autons[selectedAut]);
}