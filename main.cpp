#include "main.h"
#include <iostream>
// port, cartridge, direction, unit
pros::Controller controller(CONTROLLER_MASTER);
pros::Motor frontLeft(1, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor frontRight(2, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor backLeft(3, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor backRight(4, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor Flywheel1(5, MOTOR_GEARSET_6, true, MOTOR_ENCODER_DEGREES);
pros::Motor Flywheel2(6, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor Intake1(7, MOTOR_GEARSET_6, false, MOTOR_ENCODER_DEGREES);
pros::Motor Intake2(8, MOTOR_GEARSET_6, true, MOTOR_ENCODER_DEGREES);

int fPwr, tPwr; //forward and turn power
int rYaxis, lXaxis; //controller axis (plural form)

void initialize() {



}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */

void chas_move(float lspeed, float rspeed){
	frontRight.move(rspeed);
	backRight.move(rspeed);
	frontLeft.move(lspeed);
	backLeft.move(lspeed);
}
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {

	while (true){

		if(controller.get_digital(DIGITAL_X)){
			Flywheel1.move(0);
			Flywheel2.move(0);
		}
		else{
			Flywheel1.move(127);
			Flywheel2.move(127);
		}

		rYaxis = controller.get_analog(ANALOG_RIGHT_Y);
		lXaxis = controller.get_analog(ANALOG_LEFT_X);
		fPwr = (abs(rYaxis) > 4) ? rYaxis : 0;
		tPwr = (abs(lXaxis) > 4) ? lXaxis : 0;
		chas_move(fPwr + tPwr, fPwr - tPwr);

		pros::delay(20);
	}

}
