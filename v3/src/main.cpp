#include "global.hpp"
#include "master.hpp"

// - globals
void (*auton)();

void initialize() 
{
	// - calibration and sensor init
	glb::imu.reset();
	glb::controller.clear();

	// - autSelector
	auton = autonSelector();
	
}

void disabled() {}

void competition_initialize() {}

void autonomous() 
{
	auton();
}

void opcontrol() 
{
	while (true) 
	{
		normal();
		pros::delay(20);
	}
}