#include "global.hpp"
#include "cata.hpp"
#include "master.hpp"

// - globals
void (*auton)();

void initialize() 
{
	// - calibration and sensor init
	glb::imu.reset();
	glb::controller.clear();

	// - autSelector
	// auton = autonSelector();

	// - tasks
	pros::Task intake(intake::control);
	pros::Task cata(cata::control);
}

void disabled() {}

void competition_initialize() {}

void autonomous() 
{
	auton();
}

void opcontrol() 
{
	cata::curr = cata::reloading;
	intake::curr = intake::idling;
	while (true) 
	{
		normal();
		pros::delay(20);
	}
}