#include "flywheel.hpp"
#include "main.h"
#include "global.hpp"
#include "chassis.hpp"
#include "autons.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "util.hpp"
#include "autoaim.hpp"


util::timer decelTimer; 
util::timer indexTimer;
util::timer doubleTap;
util::timer fowrardTimer;
double prevSpeed;
bool toggled;

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

// void felixFw(double input) 
// {
//     double kP = 0.8;
 
//     const double SCALE = 1.27;
//     double speed = robot::flywheel.getSpeed() / 6;
//     if (speed < input) 
//     {
//         double error = input - speed;
//         robot::flywheel.spin((input + error*kP) * SCALE);
//         // glb::controller.print(0, 0, "%f", input + error*kP);
//     } 
    
//     else 
//     {
//         robot::flywheel.spin(input * SCALE);
//     }
// }

void felixControl()
{
    // chassis
    double lStick = -glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2) && (doubleTap.time() <= 150))
    {
        glb::controller.rumble(".");
        robot::angler.toggle();

        if(robot::angler.state)
        {
            flywheel::target += 35;
            
        }

        else
        {
            flywheel::target -= 35;
        }
    }

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
        toggled = !toggled;
    }

    if(std::abs(lStick) < 9 && std::abs(rStick) < 9 && decelTimer.time() <= 8000 && toggled)
    {
        static util::pid pid(util::pidConstants(0.6, 0.1, 0, 0.1, 0.3, 1000), 1000);
        pros::vision_object_s_t goal = glb::vision.get_by_sig(0, 2);
        int x = goal.left_coord;
        int error = 66-x;
        // if(std::abs(error) < 1)
        // {
        //     error = 0;
        // }

        int vel = pid.out(error);
        
        robot::chass.spinDiffy(-vel, vel);
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        doubleTap.start();
        if(!robot::angler.state)
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 340;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
            {
                flywheel::target = 580;
            }
        
            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 470;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 400;
            }

            else
            {
                flywheel::target = prevSpeed;
            }
        }

        else
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 375;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 530;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 450;
            }
            
            else
            {
                flywheel::target = prevSpeed;
            }
        }
        decelTimer.start();

    }

    else
    {

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2) && robot::tsukasa.state)
        {
            robot::tsukasa.toggle();
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            robot::intake.spin(127);
        }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            // flywheel::ff = true;
            robot::intake.spin(-60);
            decelTimer.start();
        }

        else
        {
            robot::intake.stop("c");
        }

        if(!glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            indexTimer.start();
            fowrardTimer.start();
            flywheel::ff = -1;
            // inRange.start();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1))
        {
            flywheel::ff = 3;
        }

        // if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        // {
        //     robot::tsukasa.toggle();
        // }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        {
            robot::tsukasa.toggle();
        }
    }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
        {
            robot::plane.toggle();
            robot::cata.toggle();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
        {
            robot::plane.toggle();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
        {
            robot::cata.toggle();
        }
    // }

    //aut

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        robot::angler.toggle();

        if(robot::angler.state)
        {
            flywheel::target -= 40;
        }

        else
        {
            flywheel::target += 40;
        }
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
        // intake::toggle(true);
    }
    

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        // double target = 180 - absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        // target = target >= 0 ? target : 180 + fabs(target); //conver to 0-360
        // glb::controller.print(0, 0, "%f", target);

        // // chas::spinTo(135, 1000, 1);
        // chas::spinTo(target, 5000, 1);
        // robot::intake.spin(-127);
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
    

    // if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    // {
    //     flywheel::ff = true;
    // }
    

    if (decelTimer.time() > 9000)
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
    
    else if(flywheel::target != 0)
    {
        // glb::controller.rumble(".");
        prevSpeed = flywheel::target;
    }
}

void keejControl()
{
    // chassis
    int deadband = 1;

    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    // double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    // double rStick = glb::controller.get_analog(ANALOG_RIGHT_Y);


    // robot::chass.spinDiffy(lStick, rStick);
    // robot::chass.spinDiffy(lStick,rStick);

    // if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    // {
    //     autoAim();
    // }

    if(std::abs(lStick) < deadband)
    {
        // rStick = rStick/2;
        // rStick = -11.2694276696 * sqrt(-rStick + 127);
        // rStick = pow(rStick,2)/127;

        // if(rStick < 78.49)
        // {
        //     rStick = (-11.2694276696 * sqrt(-rStick + 127)) + 127;
        // }
        // else
        // {
        //     rStick = pow(rStick,2)/127;
        // }
        // rStick = pow(rStick,2)/127 * util::sign(rStick);
        if(rStick > 0)
        {
            rStick = (-11.2694276696 * sqrt(-rStick + 127)) + 127;
        }

        else
        {
            rStick = -pow(rStick/11.2694276696,2);
        }
    }

    // robot::chass.spinDiffy(lStick, rStick);
    robot::chass.spinDiffy(lStick+rStick,lStick-rStick);
    
    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
        toggled = !toggled;
    }
    
    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
    {
        glb::red = !glb::red;
    }

    if(std::abs(lStick) < 9 && std::abs(rStick) < 9 && decelTimer.time() <= 8000 && toggled)
    {
        static util::pid pid(util::pidConstants(0.6, 0.1, 0, 0.1, 0.3, 1000), 1000);
        pros::vision_object_s_t goal;
        int center;
        if(glb::red)
        {
            pros::vision_object_s_t goal = glb::vision.get_by_sig(0, 1);
            center = 87;
        }
        else
        {
            pros::vision_object_s_t goal = glb::vision.get_by_sig(0, 2);
            center = 68;
        }

        int x = goal.left_coord;
        int error = center-x;
        // if(std::abs(error) < 1)
        // {
        //     error = 0;
        // }

        int vel = pid.out(error);
        
        robot::chass.spinDiffy(-vel, vel);
    }
    // if(std::abs(lStick) < 10 && std::abs(rStick) < 10)
    // {
    //     int kp = 0.6;
    //     pros::vision_object_s_t goal = glb::vision.get_by_sig(0, 2);
    //     int x = goal.left_coord;
    //     if(std::abs(66-x) < 3)
    //     {
    //         kp = 0;
    //     }

    //     int vel = (66-x) * kp;
        
    //     robot::chass.spinDiffy(-vel, vel);
    // }
    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2) && (doubleTap.time() <= 150))
    {
        glb::controller.rumble(".");
        robot::angler.toggle();

        if(robot::angler.state)
        {
            flywheel::target += 45;
        }

        else
        {
            flywheel::target -= 45;
        }
    }

    else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        doubleTap.start();
        if(!robot::angler.state)
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 340;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
            {
                flywheel::target = 700;
            }
        
            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 470;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 400;
            }

            else
            {
                flywheel::target = prevSpeed;
            }
        }

        else
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 375;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 530;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 450;
            }

            else
            {
                flywheel::target = prevSpeed;
            }
        }

        decelTimer.start();
    }

    else
    {
        // robot::chass.spinDiffy(lStick+rStick,lStick-rStick);

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
            // flywheel::ff = true;
            robot::intake.spin(-60);
            decelTimer.start();
            decelTimer.startTime += 3000;
        }


        else
        {
            robot::intake.stop("c");    
        }

        if(!glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            indexTimer.start();
            fowrardTimer.start();
            flywheel::ff = -1;
            // inRange.start();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        {
            flywheel::ff = 3;
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
        {
            robot::angler.toggle();

            if(robot::angler.state)
            {
                flywheel::target += 35;
            }

            else
            {
                flywheel::target -= 35;
            }
        }

        else if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2))
        {
            robot::tsukasa.toggle();
        }
    }

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
    {
        robot::plane.toggle();
        robot::cata.toggle();
    }

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
    {
        robot::plane.toggle();
    }

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
    {
        robot::cata.toggle();
    }

    //aut

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        flywheel::target = 0;
    }
    

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    {
        // double target = 180 - absoluteAngleToPoint(glb::pos, util::coordinate(0,0));
        // target = target >= 0 ? target : 180 + fabs(target); //conver to 0-360
        // glb::controller.print(0, 0, "%f", target);
        // intake::hardToggle();
        // robot::intake.spin(-127);
        // autoAim(900,1);
        chas::arcTurn(PI/4,700,700, 1, util::pidConstants(2.8,0.4,20,0.05,5,100));
        chas::arcTurn(0.000001,700,1200, -1, util::pidConstants(2.8,0.4,20,0.05,5,100));
        // chas::arcTurn(PI/2,-400,1000, -1, util::pidConstants(2.8,0.4,20,0.05,5,100));
        // chas::arcTurn(-PI/4,400,1800, util::pidConstants(2.8,0.2,20,0.05,5,100));
    }
    

    if (decelTimer.time() > 9000)
    {
        if (flywheel::target > 340)
        {
            flywheel::target -= 0.5;
        }

        else if (flywheel::target != 0)
        {
            flywheel::target = 340;
        }

        else
        {
            flywheel::target = 0;
        }

        // flywheel::target > 300 ? flywheel::target -= 0.5 : flywheel::target != 0 ? flywheel::target = 300 : flywheel::target = 0;
    }
    else if(flywheel::target != 0)
    {
        // glb::controller.rumble(".");
        prevSpeed = flywheel::target;
    }
}

void anthony()
{
    // chassis
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_Y);


    // robot::chass.spinDiffy(lStick, rStick);
    robot::chass.spinDiffy(lStick,rStick);

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2) && doubleTap.time() <= 300)
    {
        robot::angler.toggle();

        if(robot::angler.state)
        {
            flywheel::target += 40;
        }

        else
        {
            flywheel::target -= 40;
        }
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    {
        doubleTap.start();
        if(robot::angler.state)
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 350;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_A))
            {
                flywheel::target = 580;
            }
        
            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 470;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 400;
            }
        }

        else
        {
            if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
            {
                flywheel::target = 390;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
            {
                flywheel::target = 500;
            }

            else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
            {
                flywheel::target = 440;
            }
        }
        decelTimer.start();

        // robot::chass.spinDiffy(lStick + (rStick/2), lStick - (rStick/2));
    }

    else
    {
        // robot::chass.spinDiffy(lStick+rStick,lStick-rStick);


        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1) && robot::tsukasa.state)
        {
            robot::tsukasa.toggle();
        }

        if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
        {
            robot::intake.spin(127);
        }

        // else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        // {
        //     if(flywheel::gError < 15)
        //     {
        //         robot::intake.spin(-80);
        //     }

        //     else
        //     {
        //         robot::intake.stop("c");
        //     }
        //     decelTimer.start();
        //     decelTimer.startTime += 3000;
        // }

        else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            // flywheel::ff = true;
            robot::intake.spin(-60);
            decelTimer.start();
            decelTimer.startTime += 3000;
        }

        // else if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        // {
        //     if(indexTimer.time() >= 150)
        //     {
        //         if (indexTimer.time() >= 200)
        //         {
        //             indexTimer.start();
        //         }

        //         robot::intake.stop("b");
        //     }

        //     else 
        //     {
        //         robot::intake.spin(-80);
        //     }

        //     decelTimer.start();
        //     decelTimer.startTime += 3000;
        // }

        else
        {
            robot::intake.stop("c");
        }

        if(!glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
        {
            indexTimer.start();
            fowrardTimer.start();
            flywheel::ff = -1;
            // inRange.start();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
        {
            flywheel::ff = 3;
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
            robot::plane.toggle();
            robot::cata.toggle();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
        {
            robot::plane.toggle();
        }

        if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
        {
            robot::cata.toggle();
        }
    // }

    //aut

    if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y))
    {
        robot::angler.toggle();

        if(robot::angler.state)
        {
            flywheel::target += 40;
        }

        else
        {
            flywheel::target -= 40;
        }
    }

    if(glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    {
        intake::toggle(true);
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
    

    // if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1))
    // {
    //     flywheel::ff = true;
    // }
    

    if (decelTimer.time() > 6000)
    {
        if (flywheel::target > 320)
        {
            flywheel::target -= 0.5;
        }

        else if (flywheel::target != 0)
        {
            flywheel::target = 320;
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
    }

    else
    {
        glb::driver = false;
    }

    glb::red = color % 2 == 0 ? true : false;

    pros::delay(200);

    return(autons[selectedAut]);
}