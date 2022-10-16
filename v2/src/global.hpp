#ifndef __GLOBAL__
#define __GLOBAL__

#include "main.h"
#include "pros/adi.hpp"
#include "util.hpp"
#include <string>

namespace group
{
    class mtrs;
    class chassis;
    class pis;
    class imu;
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

    //pistons
    pros::ADIDigitalOut derrick(6);
    pros::ADIDigitalOut expansion(5);

    // sensors
    pros::Imu imu(5);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);   
    pros::ADIEncoder leftEncoder(3,4,false);
    pros::ADIEncoder horizEncoder(1,2,false);
    pros::Optical optical(20);
    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
    util::coordinate pos = util::coordinate(0,0);
    util::timer matchTimer;
    bool red;
    bool driver;
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
        std::string name;

    public:

        // mtrs(const std::initializer_list<pros::Motor> & motorsList)

        mtrs(const std::vector<pros::Motor> & motorsList, std::string title) : motors(motorsList) , name(title){}

        void spin(double volts = 127)
        {
            // printf("%s.spin(%f);\n", name.c_str(),volts);

            for (int i=0; i < motors.size(); i++)
            {
                motors[i].move(volts);
            }
        }

        void stop(std::string brakeMode)
        {
            // printf("%s.stop(%s);\n", name.c_str(),"brakeMode");
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < motors.size(); i++)
            {
                motors[i].set_brake_mode(brakeType);
                motors[i].brake();
            }
        }

        void setBrake(std::string brakeMode)
        {
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < motors.size(); i++)
            {
                motors[i].set_brake_mode(brakeType);
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

        void spinDist(double deg, double vel, std::string brakeMode)
        {
            this->reset();

            while(deg < this->getRotation())
            {
                this->spin(vel);
            }

            this->stop(brakeMode);
        }

        void spinFor(double time, double vel, std::string brakeMode)
        {
            util::timer timer;

            while(timer.time() < time)
            {
                this->spin(vel);
            }

            this->stop(brakeMode);
        }
};

class group::chassis : public group::mtrs
{
    public:

        // chassis(const std::initializer_list<pros::Motor> & motors) : mtrs(motors){}
        chassis(const std::vector<pros::Motor> & motorsList, std::string title) : mtrs(motorsList,title){}

        void spinDiffy(double rvolt, double lvolt)
        {
            // printf("%s.spinDiffy(%f,%f);\n", name.c_str(), rvolt, lvolt);

            for (int i=0; i < motors.size()/2; i++)
            {
                motors[i].move(rvolt);
                motors[i+motors.size()/2].move(lvolt);
                // glb::controller.print(0,0,"%f", rvolt);
            }
        }
};

class group::pis
{
    private:

        std::vector<pros::ADIDigitalOut> pistons;
    
    public:
        bool state;
        
        pis(std::vector<pros::ADIDigitalOut> p, bool s) : pistons(p), state(s)
        {
            setState(s);
        }

        void toggle()
        {
            state = !state;

            for(int i = 0; i < pistons.size(); i++)
            {
                pistons[i].set_value(state);
            }
        }

        void setState(bool iState)
        {
            state = iState;

            for(int i = 0; i < pistons.size(); i++)
            {
                pistons[i].set_value(state);
            }
        }
};

class group::imu
{
    private:

        pros::IMU inertial;
        double initHeading;
    
    public:

        imu(pros::IMU imu, double heading): inertial(imu), initHeading(heading) {}

        double degHeading()
        {
            double t = inertial.get_heading() + initHeading;
            return(t <= 360 ? t : 0 + (t-360));
        }

        double radHeading()
        {
            double t = inertial.get_heading() + initHeading;
            return(t <= 360 ? util::dtr(t) : util::dtr((t-360)));
        }

        void init(double heading)
        {
            initHeading = heading;
        }
};

namespace robot
{
    std::vector<pros::Motor> chassisMotors{glb::frontLeft,glb::backLeft, glb::frontRight,glb::backRight};
    std::vector<pros::Motor> intakeMotors{glb::intake1,glb::intake2};
    std::vector<pros::Motor> flywheelMotors{glb::fw1,glb::fw2};
    std::vector<pros::ADIDigitalOut> intakePistons{glb::derrick};
    std::vector<pros::ADIDigitalOut> expansionPistons{glb::expansion};
    
    group::chassis chass(chassisMotors,"chass");
    group::mtrs intake(intakeMotors, "intake");
    group::mtrs flywheel(flywheelMotors, "flywheel");
    group::pis tsukasa(intakePistons,false);
    group::pis expansion(expansionPistons, false);
    group::imu imu(glb::imu, 0);

} 

#endif