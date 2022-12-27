#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include <functional>
#include <vector>

// lib::listener* con;

void normal()
{
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    bool L1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool L2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool R1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool R2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool LIMIT = glb::limit.get_value();

    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;
    // glb::controller.print(1, 1, "%f,%f", rvolt,lvolt);

    robot::chassisMotors.spinDiffy(lvolt,rvolt);
    // glb::frontRight.move_voltage(rvolt)

    if(robot::pto.state)
    // if(true)
    {
        if(R1)
        {
            robot::itsuki.spin(-100);
        }

        else if (L1) 
        {
            robot::itsuki.spin(100);
        }

        // else if (!LIMIT)
        // {
        //     robot::itsuki.spin(-100);
        // }

        else
        {
            robot::itsuki.stop('c');
        }

        if(R2)
        {
            robot::pto.toggle();
        }
    }

    else
    {
        robot::itsuki.spinDiffy(rvolt, lvolt);

        if(R1 || L1)
        {
            robot::pto.toggle();
        }

        if(R2)
        {
            robot::pto.toggle();
        }
    }

}

void (*autonSelector())() //NOLINT
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

    glb::red = color % 2 == 0 ? true : false;

    pros::delay(200);

    return(autons[selectedAut]);
}