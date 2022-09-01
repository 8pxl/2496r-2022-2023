#include "main.h"
#include "global.hpp"
#include "chassis.hpp"
#include "util.hpp"
#include "autons.hpp"
#include "intake.hpp"

util::timer decelTimer; 

void driveContol()
{
    // chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

    // intake
    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {
        robot::intake.spin(127);
        rollers::noInput = false;
    }

    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {
        robot::intake.spin(-127);
        rollers::noInput = false;
    }

    else
    {
        // rollers::noInput = true;
        // rollers::spin();
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

        flywheel::target = 480;
        robot::chass.spin(80);
        robot::intake.spin(110);
        pros::delay(450);
        robot::intake.stop("c");
        chas::drive(-500, 1000, 1);
        chas::spinTo(356, 800, 0.5);
        index(1);
        pros::delay(1100);
        index(2);
        chas::spinTo(233, 1400, 1);
        robot::intake.spin(127); 
        glb::derrick.set_value(true);
        chas::drive(1300, 900, 20);
        glb::derrick.set_value(false);
        pros::delay(500);
        flywheel::target = 420;
        chas::spinTo(345, 1300, 1);
        robot::intake.stop("c");
        flywheel::target = 480;
        index(1);
        pros::delay(500);
        index(1);
        pros::delay(500);
        index(1);
        chas::drive(500, 600, 1);
        chas::spinTo(217, 1400, 1);
        robot::intake.spin(127); 
        chas::drive(6500, 2500, 20);
        chas::spinTo(270, 800, 1);
        robot::intake.stop("b"); 
        robot::chass.spin(80);
        robot::intake.spin(110);
        pros::delay(300);
        robot::intake.stop("c");
        // chas::moveTo(util::coordinate(0,0), 100000, 0.6, 0.4, 1);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        double target = absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        target = target >= 0 ? target : 180 + (180 - fabs(target)); //conver to 0-360
        glb::controller.print(0, 0, "%f", target);

        // chas::spinTo(135, 1000, 1);
        chas::spinTo(target, 5000, 1);
    }
    

    // glb::controller.print(0,0, "%f", glb::imu.get_heading());
    util::coordinate a = util::coordinate(0,0);
    // chas::moveToVel(a, 0, 0,  0);

    if (decelTimer.time() > 3000)
    {
        flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target != 0 ? flywheel::target = 300 : flywheel::target = 0;
    }
}

int autonSelector()
{
    int numAutons = autons.size() - 1;
    int selectedAut = 0;
    bool autSelected = false;

    robot::flywheel.reset();

    while(1)
    {   
        glb::controller.clear();

        if(robot::flywheel.getRotation() >= 60)
        {
            selectedAut = selectedAut == numAutons ? 0 : selectedAut++;
            robot::flywheel.spin(-2);
            robot::flywheel.reset();
        }

        if(robot::flywheel.getRotation() <= -60)
        {
            selectedAut = selectedAut == 0 ? numAutons : selectedAut --;
            robot::flywheel.spin(-2);
            robot::flywheel.reset();
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            break;
        }

        glb::controller.print(0, 0, "%s", autonNames[selectedAut]);

        // glb::controller.print(0, 0, "%f", robot::flywheel.getRotation());

    }

    return(selectedAut);
}

