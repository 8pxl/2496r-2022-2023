#ifndef __GROUPS__
#define __GROUPS__

#include "main.h"

namespace lib
{
    class mtrs
    {
        private:
            pros::motor_brake_mode_e returnBrakeType(char brakeMode);

        protected:
            std::vector<pros::Motor> motors;
            int size;

        public:
            mtrs(const std::vector<pros::Motor> & motorsList) : motors(motorsList), size(motorsList.size()){}
            
            void spin(double volts);
            void stop(char brakeMode);
            void setBrake(char brakeMode);
            void reset();
            double getSpeed();
            double getRotation();
    };

    class diffy : public lib::mtrs
    {
        public:
            diffy(const std::vector<pros::Motor> & motorsList) : mtrs(motorsList){}

            void spinDiffy(double rvolt, double lvolt);
            void spinDiffy(std::vector<double> voltages);

            std::vector<double> getDiffy();
            double getLeft();
            double getRight();
    };

    class pis
    {
        private:
            std::string name;
            std::vector<pros::ADIDigitalOut> pistons;
            bool state;
        
        public:
            
            pis(std::vector<pros::ADIDigitalOut> p, bool s, std::string title) : pistons(p), state(s), name(title)
            {
                setState(s);
            }

            void toggle();

            void setState(bool iState);

            bool getState(){return state;}
    };
}

pros::motor_brake_mode_e lib::mtrs::returnBrakeType(char brakeMode) //NOLINT
{
    return brakeMode == 'c' ? pros::E_MOTOR_BRAKE_COAST : brakeMode == 'b' ? pros::E_MOTOR_BRAKE_BRAKE : pros::E_MOTOR_BRAKE_HOLD;
}

void lib::mtrs::spin(double volts) 
{
    for (int i=0; i < size; i++)
    {
        motors[i].move(volts);
    }
}

void lib::mtrs::stop(char brakeMode) //NOLINT
{
    pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

    for (int i=0; i < size; i++)
    {
        motors[i].set_brake_mode(brakeType);
        motors[i].brake();
    }
}

void lib::mtrs::setBrake(char brakeMode) //NOLINT
{
    pros::motor_brake_mode_e brakeType = returnBrakeType(brakeMode);

    for (int i=0; i < size; i++)
    {
        motors[i].set_brake_mode(brakeType);
    }
}

double lib::mtrs::getSpeed() //NOLINT
{
    double vel = 0;

    for (int i=0; i < size; i++)
    {
        vel += motors[i].get_actual_velocity();
    }
    
    return(vel/size);
}

double lib::mtrs::getRotation() //NOLINT
{
    double rotation = 0;

    for (int i=0; i < size; i++)
    {
        rotation += motors[i].get_position();
    }
    
    return(rotation/size);
}

void lib::mtrs::reset() //NOLINT
{
    for (int i=0; i < size; i++)
    {
        motors[i].set_zero_position(0);
    }
}

void lib::diffy::spinDiffy(double rvolt, double lvolt) 
{
    int half = size/2;

    for (int i=0; i < half; i++)
    {
        motors[i].move(rvolt);
        motors[i + half].move(lvolt);
    }
}

void lib::diffy::spinDiffy(std::vector<double> voltages) 
{
    int half = size/2;

    for (int i=0; i < half; i++)
    {
        motors[i].move(voltages[0]);
        motors[i + half].move(voltages[1]);
    }
}

std::vector<double> lib::diffy::getDiffy() //NOLINT
{
    double dl = 0;
    double dr = 0;
    int half = size/2;
    
    for (int i=0; i < half; i++)
    {
        dl += motors[i].get_position();
        dr += motors[i + half].get_position();
    }
    
    return(std::vector<double> {dr/half, dl/half});
}

double lib::diffy::getLeft()
{
    double dl = 0;
    int half = size/2;
    
    for (int i=0; i < half; i++)
    {
        dl += motors[i].get_position();
    }
    
    return(dl/half);
}

double lib::diffy::getRight()
{
    double dr = 0;
    int half = size/2;
    
    for (int i = 0; i < half; i++)
    {
        dr+= motors[i+half].get_position();
    }

    return(dr/half);
}


void lib::pis::toggle() 
{
    state = !state;

    for(int i = 0; i < pistons.size(); i++)
    {
        pistons[i].set_value(state);
    }
}

void lib::pis::setState(bool iState) //NOLINT
{
    // printf("robot::%s.setState(%d);\n", name.c_str(), iState);
    state = iState;

    for(int i = 0; i < pistons.size(); i++)
    {
        pistons[i].set_value(state);
    }
}

#endif