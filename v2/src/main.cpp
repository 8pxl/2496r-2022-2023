#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "util.hpp"
#include "flywheel.hpp"

//globals
void (*auton)();
bool control = true;

void initialize() 
{
	//calibration and sensor init
	glb::imu.reset();
	// glb::optical.set_led_pwm(100);
	glb::controller.clear();
	// pros::delay(3000);

	//autSelector
	auton = autonSelector();
	
	//tasks
	pros::Task od(odom);
	// pros::Task fw(flywheel::spin);

	//fw initial vel
	flywheel::target = 0;
	
}

void disabled() {}

void competition_initialize() {}

void autonomous() 
{
	auton();
}

void opcontrol() 
{
	glb::matchTimer.start();
	int count = 0;
	int i = 6;

	while (true) 
	{

        // if(count % 1000 == 0)
        // {
        //         i++;
        // }

		// flywheel::target = 50*i;

		// count++;


		if (glb::driver) {keejControl();}

		// else {felixControl();}

		if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}

		pros::delay(20);
	}
}
