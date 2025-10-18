#include "global.hpp"
#include "cata.hpp"
#include "controls.hpp"
#include "autons.hpp"

// Global function pointers and driver selection
void (*auton)();
int driver;

/**
 * @brief Initialization function. Runs once at startup.
 * Handles IMU calibration, auton selection, and task creation.
 */
void initialize() {
    glb::imu.reset();
    glb::controller.clear();

    // Autonomous selector
    auton = autons[robot::controller.select(autons.size(), autonNames)];
    if (auton == WP) { robot::tsukasa.toggle(); }

    // Driver control selector
    driver = robot::controller.select(2, {"keej", "felix"});

    // Start subsystem tasks
    pros::Task intake(intake::control);
    pros::Task cata(cata::control);
}

/** @brief Runs the selected autonomous routine. */
void autonomous() { auton(); }

/**
 * @brief Operator control loop. 
 * Dispatches to the selected driver control scheme.
 */
void opcontrol() {
    cata::boost = false;
    intake::curr = intake::idling;
    robot::controller.setCurves(0, 8);

    while (true) {
        switch (driver) {
            case 0:
                if (glb::controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
                    auton();
                }
                keej();
                break;

            case 1:
                felix();
                break;
        }
        pros::delay(20);
    }
}
