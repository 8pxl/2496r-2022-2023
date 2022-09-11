#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "util.hpp"
#include "flywheel.hpp"

//globals
void (*auton)();

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
	(*auton)();
}

void opcontrol() 
{
	glb::red = true;

	while (true) 
	{
		driveContol();
		pros::delay(20);
	}
}