#ifndef __GLOBAL__
#define __GLOBAL__

#include "main.h"

namespace group
{
    class mtrs;
    class chassis;
}

namespace glb
{
    //motors
    pros::Motor frontLeft(3, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor frontRight(2, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backLeft(4, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor backRight(1, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake1(15, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake2(14, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor fw1(12, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor fw2(13, pros::E_MOTOR_GEARSET_06, false);

    //sensors
    pros::Imu imu(5);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);   
    pros::ADIEncoder leftEncoder(1,2,false);
    pros::ADIEncoder horizEncoder(3,4,false);
    pros::ADIEncoder rightEncoder(5,6,false);

    //variables

    double x;
    double y; 

    //init
}

class group::mtrs
{   
    private:

        pros::motor_brake_mode_e returnBrakeType(std::string brakeMode)
        {
        return brakeMode == "c" ? pros::E_MOTOR_BRAKE_COAST : brakeMode == "b" ? pros::E_MOTOR_BRAKE_BRAKE : pros::E_MOTOR_BRAKE_HOLD;
        }

    protected:
        std::vector<pros::Motor> motors;

    public:

        // mtrs(const std::initializer_list<pros::Motor> & motorsList)

        mtrs(const std::vector<pros::Motor> & motorsList) : motors(motorsList){}

        void spin(double volts = 127)
        {

            for (int i=0; i < sizeof(motors); i++)
            {
                motors[i].move(volts);
            }
        }

        void stop(std::string brakeMode)
        {
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < sizeof(motors); i++)
            {
                motors[i].set_brake_mode(brakeType);
                motors[i].brake();
            }
        }
};

class group::chassis : public group::mtrs
{
    public:

        // chassis(const std::initializer_list<pros::Motor> & motors) : mtrs(motors){}
        chassis(const std::vector<pros::Motor> & motorsList) : mtrs(motorsList){}

        void spinDiffy(double rvolt, double lvolt)
        {
            // printf("%f", rvolt);

            for (int i=0; i < motors.size()/2; i++)
            {
                motors[i].move(rvolt);
                motors[i+2].move(lvolt);
                // glb::controller.print(0,0,"%f", rvolt);
            }
        }
};

namespace robot
{
    std::vector<pros::Motor> chassisMotors{glb::frontLeft,glb::backLeft,glb::frontRight,glb::backRight};
    group::chassis chass(chassisMotors);
}

#endif