#include "global.hpp"
#include "cata.hpp"
#include "master.hpp"

// - globals
void (*auton)();
int driver;

void initialize() 
{
	// - calibration and sensor init
	glb::imu.reset();
	glb::controller.clear();

	// - autSelector
	auton = autons[robot::selector.select(autons.size(), autonNames)];
	driver = robot::selector.select(2, {"keej", "felix"});

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
		switch (driver)
		{
			case 0:
				keej();
				break;

			case 1:
				felix();
				break;
		}
		pros::delay(20);
	}
}