#include "main.h"

namespace glb
{
    //motors

    pros::Motor frontLeft(1, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor frontRight(2, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backLeft(3, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backRight(4, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake1(5, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake2(6, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor fw1(7, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor fw2(8, pros::E_MOTOR_GEARSET_06, false);

    //sensors

    pros::Imu imu(1);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);    

    //variables

    double x;
    double y; 
}

namespace group
{
    class mtrs;
    class chassis;
}

class group::mtrs
{   
    private:

        pros::motor_brake_mode_e returnBrakeType(std::string brakeMode)
        {
        return brakeMode == "c" ? pros::E_MOTOR_BRAKE_COAST : brakeMode == "b" ? pros::E_MOTOR_BRAKE_COAST : pros::E_MOTOR_BRAKE_HOLD;
        }

    public:

        std::vector<pros::Motor> motors;

        mtrs(std::vector<pros::Motor> motorsList)
        {
            motors = motorsList;   
        }

        void spin(int pct = 100)
        {
            double voltage = pct * 120;

            for (int i=0; i <= sizeof(motors); i++)
            {
                motors[i].move_voltage(voltage);
            }
        }

        void stop(std::string brakeMode)
        {
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i <= sizeof(motors); i++)
            {
                motors[i].set_brake_mode(brakeType);
                motors[i].brake();
            }
        }
};

class group::chassis : public group::mtrs
{
    public:

        chassis(std::vector<pros::Motor> motors) : mtrs(motors){}

        void spinDiffy(int rpct, int lpct)
        {
            double rvolt = rpct * 120;
            double lvolt = lpct * 120;

            for (int i=0; i <= sizeof(motors)/2; i++)
            {
                motors[i].move_voltage(rvolt);
                motors[i+2].move_voltage(lvolt);
            }
        }
};



// init
pros::Motor c[] = {glb::frontLeft,glb::backLeft,glb::frontRight,glb::backRight};

std::vector<pros::Motor> ch(c, c + sizeof(c)/ sizeof(c[0]));

group::chassis chass(ch);
 
