#include "global.hpp"
#include "cata.hpp"
#include "controls.hpp"
#include "autons.hpp"

// - globals
void (*auton)();
int driver;

void initialize() 
{
	// - calibration and sensor init
	glb::imu.reset();
	glb::controller.clear();

	// - autSelector
	auton = autons[robot::controller.select(autons.size(), autonNames)];
	if(auton == WP){robot::tsukasa.toggle();}
	driver = robot::controller.select(2, {"keej", "felix"});

	// - tasks
	pros::Task intake(intake::control);
	pros::Task cata(cata::control);
}

void autonomous() {auton();}

void opcontrol() 
{
	// cata::curr = cata::reloading;
	cata::boost = false;
	intake::curr = intake::idling;
	robot::controller.setCurves(0, 8);
	while (true) 
	{
		switch (driver)
		{
			case 0:
				if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}
				keej();
				break;

			case 1:
				felix();
				break;
		} 	
		pros::delay(20);
	}
}