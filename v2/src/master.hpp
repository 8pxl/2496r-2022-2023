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

    // intake
    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
        robot::intake.spin(127);
    }

    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        robot::intake.spin(-90);
    }

    else
    {
        robot::intake.stop("c");
    }

    // fw

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 600;
        }

        else
        {
            flywheel::target = 500;
        }

        decelTimer.start();
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 300;
        }

        else
        {
            flywheel::target = 400;
        }
        decelTimer.start();
    }

    //aut

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
    {
        // chas::moveTo(util::coordinate(0,0), 100000, 0.7, 4, 0.6);

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

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        double target = 180 - absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        target = target >= 0 ? target : 180 + fabs(target); //conver to 0-360
        glb::controller.print(0, 0, "%f", target);

        // chas::spinTo(135, 1000, 1);
        chas::spinTo(target, 5000, 1);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_X))
    {
        glb::pos = util::coordinate(103,675);
        robot::imu.init(180);


        util::pidConstants linearConstants(2.4,0,1.5,30,5);
        util::pidConstants rotationConstants(2.2,0,2.5,3,5);

        flywheel::target = 480;

        //toggle roller
        intake::toggle();

        chas::drive(-500, 1000, 1);
        chas::spinTo(176, 800, 0.5);

        intake::index(2);  
        flywheel::target = 200;
        robot::tsukasa.toggle();
        chas::moveTo(util::coordinate(200,530), 4000, linearConstants, rotationConstants, 1.2);
        robot::tsukasa.toggle();
        robot::intake.spin(127);
        pros::delay(1000);

    }

    // glb::controller.print(0,0, "%f", glb::imu.get_heading());
    // util::coordinate a = util::coordinate(0,0);
    // chas::moveToVel(a, 0, 0,  0);

    if (decelTimer.time() > 4000)
    {
        flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target != 0 ? flywheel::target = 300 : flywheel::target = 0;
    }
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

