#include "main.h"
#include "master.hpp"
#include "global.hpp"
#include "odom.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "util.hpp"
#include "flywheel.hpp"

// - globals
void (*auton)();
// bool control = true;

void initialize() 
{
	// - calibration and sensor init
	glb::imu.reset();
	glb::controller.clear();

	// - autSelector
	auton = autonSelector();
	
	// - tasks
	pros::Task od(odom);
	pros::Task fw(flywheel::spin);

	//-  fw initial vel
	flywheel::target = 0;
	
}

void disabled() {}

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
		if (glb::driver) {keejControl();}

		else {felixControl();}

		if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}

		// printf("pros::delay(20);\n");
		pros::delay(20);
	}
}