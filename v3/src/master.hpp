#include "global.hpp"
#include "autons.hpp"
#include "main.h"
#include "pros/misc.h"
#include <functional>
#include <vector>

// lib::listener* con;
lib::listener con(1);

void spin(lib::mtrs* motors, int volt)
{
    motors->spin(volt);
}

void stop(lib::mtrs* motors, char brake)
{
    motors->stop(brake);
}

void controlsInit()
{
    //functions to call
    std::function<void()> intake = [&] {spin(&robot::itsuki,127); };
    std::function<void()> cata = [&] {spin(&robot::itsuki,-127); };
    std::function<void()> stopItsuki = [&] {stop(&robot::itsuki, 'c'); };

    //keys to listen to
    lib::controllerButton R1(glb::controller, pros::E_CONTROLLER_DIGITAL_R1);
    lib::controllerButton L1(glb::controller, pros::E_CONTROLLER_DIGITAL_L2);

    //pair keys to a functiom
    lib::action onR1(&R1, intake, stopItsuki);
    lib::action onL1(&L1, cata, stopItsuki);
    lib::action onLimit(&robot::puncherLimit, cata, stopItsuki);
    // std::vector<lib::action> actions{onL1, onLimit};

    con.init({onR1, onLimit, onL1});
}

void normal()
{
    double lStick = glb::controller.get_analog(ANALOG_LEFT_Y);
    double rStick = glb::controller.get_analog(ANALOG_RIGHT_X);
    robot::chassisMotors.spinDiffy(lStick + rStick,lStick - rStick);

    con.listen();
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