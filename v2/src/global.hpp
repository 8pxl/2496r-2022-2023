#ifndef __GLOBAL__
#define __GLOBAL__


#include "../include/main.h"
#include "pros/adi.hpp"
#include "util.hpp"
#include <string>
#include <vector>

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
    pros::ADIDigitalOut cata(5);
    pros::ADIDigitalOut plane(7);
    pros::ADIDigitalOut angler(8);

    // sensors
    pros::Imu imu(5);
    pros::Controller controller(pros::E_CONTROLLER_MASTER);   
    pros::ADIEncoder leftEncoder(3,4,false);
    pros::ADIEncoder horizEncoder(1,2,false);
    pros::Optical optical(20);
    pros::Vision vision (18);
    // pros::ADIEncoder rightEncoder(5,6,false);

    // variables
    util::coordinate pos = util::coordinate(0,0);
    util::timer matchTimer(1);
    // double dl;
    // double dr;
    bool red;
    bool match = false;
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
        int size;

    public:

        // mtrs(const std::initializer_list<pros::Motor> & motorsList)

        mtrs(const std::vector<pros::Motor> & motorsList, std::string title) : motors(motorsList) , name(title), size(motorsList.size()){}

        void spin(double volts = 127)
        {
            for (int i=0; i < size; i++)
            {
                motors[i].move(volts);
            }
        }

        void stop(std::string brakeMode)
        {
            // printf("robot::%s.stop('%s');\n", name.c_str(),brakeMode.c_str());
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < size; i++)
            {
                motors[i].set_brake_mode(brakeType);
                motors[i].brake();
            }
        }

        void setBrake(std::string brakeMode)
        {
            pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

            for (int i=0; i < size; i++)
            {
                motors[i].set_brake_mode(brakeType);
            }
        }

        double getSpeed()
        {
            double vel = 0;

            for (int i=0; i < size; i++)
            {
                vel += motors[i].get_actual_velocity();
            }
            
            return(vel/size);
        }

        double getRotation()
        {
            double rotation = 0;

            for (int i=0; i < size; i++)
            {
                rotation += motors[i].get_position();
            }
            
            return(rotation/size);
        }

        void reset()
        {
            for (int i=0; i < size; i++)
            {
                motors[i].set_zero_position(0);
            }
        }

        void spinDist(double deg, double vel, std::string brakeMode)
        {
            this->reset();

            while(this->getRotation() < deg)
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
            int half = size/2;

            for (int i=0; i < half; i++)
            {
                motors[i].move(rvolt);
                motors[i + half].move(lvolt);
            }
        }

        double getLeft()
        {
            double dl = 0;
            int half = size/2;
            
            for (int i=0; i < half; i++)
            {
                dl += motors[i].get_position();
            }
            
            return(dl/half);
        }

        double getRight()
        {
            double dr = 0;
            int half = size/2;
            
            for (int i = 0; i < half; i++)
            {
                dr+= motors[i+half].get_position();
            }

            return(dr/half);
        }
};

class group::pis
{
    private:
        std::string name;
        std::vector<pros::ADIDigitalOut> pistons;
    
    public:
        bool state;
        
        pis(std::vector<pros::ADIDigitalOut> p, bool s, std::string title) : pistons(p), state(s), name(title)
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
            // printf("robot::%s.setState(%d);\n", name.c_str(), iState);
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
    std::vector<pros::ADIDigitalOut> expansionPistons{glb::cata};
    std::vector<pros::ADIDigitalOut> planePistons{glb::plane};
    std::vector<pros::ADIDigitalOut> anglerPistons{glb::angler};
    
    group::chassis chass(chassisMotors,"chass");
    group::mtrs intake(intakeMotors, "intake");
    group::mtrs flywheel(flywheelMotors, "flywheel");
    group::pis tsukasa(intakePistons,false, "tsukasa");
    group::pis cata(expansionPistons, false, "cata");
    group::pis plane(planePistons, false, "plane");
    group::pis angler(anglerPistons, false, "angler");
    group::imu imu(glb::imu, 0);

} 

#endif