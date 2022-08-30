#ifndef __GLOBAL__
#define __GLOBAL__

#include "main.h"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"

namespace group
{
    class mtrs;
    class chassis;
}

namespace glb
{
    // motors
    pros::Motor frontLeft(3, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor frontRight(2, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor backLeft(4, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor backRight(1, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake1(15, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor intake2(14, pros::E_MOTOR_GEARSET_06, false);
    pros::Motor fw1(12, pros::E_MOTOR_GEARSET_06, true);
    pros::Motor fw2(13, pros::E_MOTOR_GEARSET_06, false);

    // sensors
    pros::Imu imu(5);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);   
    pros::ADIEncoder leftEncoder(3,4,false);
    pros::ADIEncoder horizEncoder(1,2,false);
    pros::Optical optical(20);
    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables

    util::coordinate pos = util::coordinate(0,0);
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

            for (int i=0; i < motors.size(); i++)
            {
                motors[i].move(volts);
            }
        }

        void stop(std::string brakeMode)
        {
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < motors.size(); i++)
            {
                motors[i].set_brake_mode(brakeType);
                motors[i].brake();
            }
        }

        double getSpeed()
        {
            double vel = 0;

            for (int i=0; i < motors.size(); i++)
            {
                vel += motors[i].get_actual_velocity();
            }
            
            return(vel/motors.size());
        }

        double getRotation()
        {
            double rotation = 0;

            for (int i=0; i < motors.size(); i++)
            {
                rotation += motors[i].get_position();
            }
            
            return(rotation/motors.size());
        }

        void reset()
        {
            for (int i=0; i < motors.size(); i++)
            {
                motors[i].set_zero_position(0);
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
    std::vector<pros::Motor> intakeMotors{glb::intake1,glb::intake2};
    std::vector<pros::Motor> flywheelMotors{glb::fw1,glb::fw2};
    
    group::chassis chass(chassisMotors);
    group::mtrs intake(intakeMotors);
    group::mtrs flywheel(flywheelMotors);
}

namespace flywheel
{
    double target;
    
    void spin()
    {
        while (true)
        {
            double currSpeed = robot::flywheel.getSpeed();
            double diff = target - currSpeed;

            if (std::abs(diff) < 50)
            {
                robot::flywheel.spin((currSpeed += diff/2) * 127 / 600);
                if (target == 400 || target == 600)
                {
                    glb::controller.rumble("-");
                }
            }

            else
            {
                robot::flywheel.spin(target * 127 / 600);
            }
            // glb::controller.print(0, 0, "%f", target);
        }
    }

}

namespace rollers
{
    bool red = true;
    void spin()
    {
        if(red)
        {   
            glb::controller.print(0, 0, "%f", glb::optical.get_hue());

            if(glb::optical.get_hue() >= 200)
            {
                robot::intake.spin(80);
            }

            else if(glb::optical.get_hue() <= 10)
            {
                robot::intake.spin(-80);
                pros::delay(100);
            }

            else
            {
                robot::intake.stop("c");
            }

        }
    }
}

#endif