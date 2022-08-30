#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "chassis.hpp"
#include "util.hpp"

//globals
int auton;

void on_center_button() {}

void initialize() 
{
	//calibration and sensor init
	glb::imu.reset();
	glb::optical.set_led_pwm(100);

	//tasks
	pros::Task od(odom);
	pros::Task fw(flywheel::spin);
	pros::Task rl(rollers::spin);

	//fw initial vel
	flywheel::target = 0;
	
	//autSelector
	auton = autonSelector();
}

void disabled() {}

void competition_initialize() {}

void autonomous() 
{
	autons[auton];
}

void opcontrol() 
{
	while (true) 
	{
		driveContol();
		pros::delay(20);
	}
}
