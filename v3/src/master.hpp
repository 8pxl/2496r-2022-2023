#include "global.hpp"
#include "autons.hpp"

void keejControl(){};

void felixControl(){};

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