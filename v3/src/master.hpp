#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include "cata.hpp"
#include "intake.hpp"
#include <functional>

#include <vector>


void test()
{
    robot::itsuki.spin(-127);
}

void normal()
{

    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);

    bool L1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1);
    bool NL1 = glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1);
    bool L2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2);
    bool R1 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1);
    bool R2 = glb::controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2);
    bool limit = glb::limit.get_value();
    std::cout << glb::limit.get_value() << std::endl;

    double rvolt = lStick - rStick;
    double lvolt = lStick + rStick;

    robot::chass.spinDiffy(lvolt,rvolt);

    if(R1)
    {
        robot::itsuki.spin(127);
    }

    // else if(R2)
    // {
    //     robot::itsuki.spin(-127);
    // }

    // else
    // {
    //     robot::itsuki.stop('c');
    // }

    else if(cata::curr == cata::idle)
    {
        robot::itsuki.stop('c');
        std::cout << "curr state: idle, stopping intake!" << std::endl;
    }
    
    if(NL1)
    {
        cata::fire();
    }

    if(R2)
    {
        cata::pause();
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