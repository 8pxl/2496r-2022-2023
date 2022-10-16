#include "flywheel.hpp"
#include "main.h"
#include "global.hpp"
#include "chassis.hpp"
#include "pros/misc.h"
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

void felixFw(double input) 
{
    double kP = 0.8;
 
    const double SCALE = 1.27;
    double speed = robot::flywheel.getSpeed() / 6;
    if (speed < input) 
    {
        double error = input - speed;
        robot::flywheel.spin((input + error*kP) * SCALE);
        // glb::controller.print(0, 0, "%f", input + error*kP);
    } 
    
    else 
    {
        robot::flywheel.spin(input * SCALE);
    }
}


void felixControl()
{
    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) 
    {
        felixFw(60);
    }
    
    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) 
    {
        felixFw(65);
    }

    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        felixFw(70);
    }

    else 
    {
        felixFw(0);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    {//index
        robot::intake.spin(-50.8);
    }
    
    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    {// intake
        robot::intake.spin(127);
    }

    else
    {
        robot::intake.stop("c");
    }

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B))
    {
        robot::tsukasa.toggle();
    }

    // if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    // {
    //     // robot::expans
    // }

    double lstick = -glb::controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) * 0.7874015748;
    double rstick = -glb::controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) * 0.7874015748;
    double axis1pow;

    if(rstick > 0)
    {
        axis1pow = 0.01*(pow(rstick, 2));
    }

    else
    {
        axis1pow = -0.01*(pow(rstick, 2));
    }

    double leftMotorSpeed = lstick + axis1pow;
    double rightMotorSpeed = lstick - axis1pow;  
        
    robot::chass.spinDiffy(rightMotorSpeed * 1.27, leftMotorSpeed*1.27);
    
}


void keejControl()
{
    // chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);


    // robot::chass.spinDiffy(lStick, rStick);


    // felixFw


    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            flywheel::target = 380;
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            flywheel::target = 580;
        }
    
        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::target = 480;
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            flywheel::target = 400;
        }

        decelTimer.start();

        robot::chass.spinDiffy(lStick + (rStick/2), lStick - (rStick/2));
    }

    else
    {
        robot::chass.spinDiffy(lStick+rStick,lStick-rStick);


            if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1) && robot::tsukasa.state)
            {
                robot::tsukasa.toggle();
            }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            robot::intake.spin(127);
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            // if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
            // {
            //     flywheel::target += flywheel::target/4;
            // }

            robot::intake.spin(-50);
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

    // if(glb::matchTimer.time() >= 50000)
    // {
        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
        {
            robot::expansion.toggle();
        }
    // }

    //aut

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
    {
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


    // if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
    // {
    //     skills();
    // }

    // if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
    // {
    //     nearHalf();
    // }

    // if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
    // {
    //     wp();
    // }

    // if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
    // {
    //     farHalf();
    // }
    


    if (decelTimer.time() > 6000)
    {
        if (flywheel::target > 300)
        {
            flywheel::target -= 0.5;
        }

        else if (flywheel::target != 0)
        {
            flywheel::target = 300;
        }

        else
        {
            flywheel::target = 0;
        }

        // flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target != 0 ? flywheel::target = 300 : flywheel::target = 0;
    }
}

void (*autonSelector())()
{

    int numAutons = autons.size() - 1;
    int selectedAut = 0;
    bool autSelected = false;
    int color = 1;
    int driver = 0;

    while(1)
    {   
        // glb::controller.clear();

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
        {
            if (selectedAut != numAutons)
            {
                selectedAut++;
            }
            
            else
            {
                selectedAut = 0;
            }

            // selectedAut = selectedAut == numAutons ? 0 : selectedAut++;
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            if (selectedAut != 0)
            {
                selectedAut--;
            }

            else
            {
                selectedAut = numAutons;
            }

            // selectedAut = selectedAut == 0 ? numAutons : selectedAut --;
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            break;
        }

        glb::controller.print(0, 0, "%s         ", autonNames[selectedAut]);

        pros::delay(50);

    }

    pros::delay(200);

    while(1)
    {   
        
        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT) || glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            color += 1;
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            break;
        }

        glb::controller.print(0, 0, "%s   ", (color % 2 == 0) ? "red" : "blue");

        pros::delay(50);
    }
    
    pros::delay(200);

    while(1)
    {   
        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT) || glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
        {
            driver += 1;
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            break;
        }

        glb::controller.print(0, 0, "%s   ", (driver % 2 == 0) ? "keej" : "felix");

        pros::delay(50);

    }
    
    if (driver % 2 == 0)
    {
        glb::driver = true;
        pros::Task fw(flywheel::spin);
    }

    else
    {
        glb::driver = false;
    }

    glb::red = color % 2 == 0 ? true : false;

    pros::delay(200);

    return(autons[selectedAut]);
}