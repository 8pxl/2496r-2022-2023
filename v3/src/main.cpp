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