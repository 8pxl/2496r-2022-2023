#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "pros/misc.h"
#include "util.hpp"
#include "flywheel.hpp"

//globals
void (*auton)();
bool control = true;

void initialize() 
{
	//calibration and sensor init
	glb::imu.reset();
	glb::optical.set_led_pwm(100);
	glb::controller.clear();
	pros::delay(3000);

	//autSelector
	// auton = autonSelector();
	
	//tasks
	pros::Task od(odom);
	pros::Task fw(flywheel::spin);

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
	glb::red = true;

	while (true) 
	{
		if (control) {keejControl();}

		else {felixControl();}

		if (glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {control = !control;}

		pros::delay(20);
	}
}