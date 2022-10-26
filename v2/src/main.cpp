#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"
#include "flywheel.hpp"
#include <algorithm>

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
	pros::Task fw(flywheel::spin);

	//fw initial vel
	flywheel::target = 0;
	
}

void disabled() 
{
}

//hello keijay, :DDDD
// lucy was here
//cool code font :oooooo

void competition_initialize() {}

void autonomous() 
{
	glb::match = false;
	auton();
}

void opcontrol() 
{
	while (true) 
	{
		// for (int i = 7; i < 13; i ++)
		// {
		// 	flywheel::target = i*50;
		// 	pros::delay(10000);
		// }

		if (glb::driver) {keejControl();}

		else {felixControl();}

		if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}

		printf("pros::delay(20);\n");
		pros::delay(20);
	}
}
