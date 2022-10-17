#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "pros/rtos.hpp"
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
	pros::Task fw(flywheel::spin);
	auton();
}

void opcontrol() 
{
	bool temp = glb::driver;
	bool temp2 = glb::match;
	glb::driver = false;
	glb::match = true;
	pros::delay(100);

	glb::driver = temp;
	glb::match = true;
	pros::Task fwd(flywheel::spin);
	
	while (true) 
	{
		if (glb::driver) {keejControl();}

		else {felixControl();}

		// if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}

		pros::delay(20);
	}
}
