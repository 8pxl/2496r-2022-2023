#include "main.h"
#include "chassis.hpp"
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
    pros::vision_signature_s_t GOAL = pros::Vision::signature_from_utility (2, -2307, -1597, -1952, 8373, 9299, 8836, 8.200, 1);
    glb::vision.set_signature(2, &GOAL);

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

		// double dl = 0;
		// double dr = 0;
		// double rotation;
		// double delta;
		// for (int i = 0; i < 10; i++)
		// {
		// 	rotation = robot::imu.degHeading();
		// 	robot::chass.reset();
		// 	chas::spinTo(robot::imu.degHeading() + 90, 1400);
		// 	delta = util::minError(robot::imu.degHeading(), rotation);
		// 	dl += (robot::chass.getLeft() * 360) / (delta * 2 * PI);
		// 	dr += (robot::chass.getRight() * 360) / (delta * 2 * PI);

		// 	printf("%f,%f", dl, dr);
		// 	glb::controller.print(0,0,"%f,%f", dl, dr);
		// }

		// chas::arcTurn(PI/2, 500, 1500,util::pidConstants(0.5,0,0,0,0,0));


		if (glb::driver) 
		{
			if(glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){auton();}
			keejControl();
		}

		else {felixControl();}

		// printf("pros::delay(20);\n");
		pros::delay(20);
	}
}