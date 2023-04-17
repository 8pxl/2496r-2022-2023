#ifndef __UTIL__
#define __UTIL__

#include "main.h"
#include "pros/misc.h"
#include <cmath>
#include <vector>

#define PI 3.14159265358979323846
#define ALLBUTTONS {pros::E_CONTROLLER_DIGITAL_L1, pros::E_CONTROLLER_DIGITAL_L2, pros::E_CONTROLLER_DIGITAL_R1, pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_UP, pros::E_CONTROLLER_DIGITAL_DOWN, pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT, pros::E_CONTROLLER_DIGITAL_X, pros::E_CONTROLLER_DIGITAL_B, pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A}
typedef void(*fptr)();

namespace util
{
    class timer;
    class coordinate;
    class pose;
    class bezier;
    class pidConstants;
    class pid;
    class movingAverage;
    class timeRange;
    struct controllerOutputs;
    class controller;
    // enum buttons;
    // struct args;
    // struct action;
    double dtr(double input);
    double rtd(double input);
    int dirToSpin(double target,double currHeading);
    double minError(double target, double current);
    double distToPoint(util::coordinate p1, util::coordinate p2);
    double mod(double a, double b);
    double absoluteAngleToPoint(util::coordinate pos, util::coordinate point);
    double imuToRad(double heading);
    double sign(double a);
}

class util::timer
{

    public:
    
        int startTime = 0;

        timer()
        {
            start();
        }

        timer(int a){}

        void start()
        {
            startTime = pros::millis();
        }

        int time()
        {
            return (pros::millis() - startTime);
        }

};

class util::coordinate
{
    public:
        double x;
        double y;

        coordinate(double px, double py)
        {
            x = px;
            y = py;
        }

        coordinate();
};

class util::pose
{
    public:

        util::coordinate pos;
        double heading;

        pose(util::coordinate p, double h) : pos(p), heading(h){}
};

class util::bezier
{
    private:
        coordinate p0;
        coordinate p1;
        coordinate p2;
        coordinate p3;
        double initialWeight;
        double finalWeight;
        double initialHeading;
        double finalHeading;

    public:
        bezier(coordinate first, coordinate last, double initialWeight, double finalWeight, double initialHeading, double finalHeading)
        {
            p0 = first;
            p1 = coordinate(first.x + sin(initialHeading) * initialWeight, first.y + cos(initialHeading) * initialWeight);
            p2 = coordinate(last.x + sin(PI/2 + (PI/2-finalHeading)) * -1 * finalWeight, last.y + cos(PI/2 + (PI/2-finalHeading)) * -1*finalWeight);
            p3 = last;
        }
        
        coordinate solve(double t)
        {
            double omt = 1-t;
            double x0 = p0.x;
            double x1 = p1.x;
            double x2 = p2.x;
            double x3 = p3.x;
            double y0 = p0.y;
            double y1 = p1.y;
            double y2 = p2.y;
            double y3 = p3.y;
            return coordinate( pow(omt,3) * x0 + 3 * pow(omt,2) * t * x1 + 3*omt * pow(t,2) * x2 + pow(t,3) * x3, pow(omt,3) * y0 + 3 * pow(omt,2) * t * y1 + 3*omt * pow(t,2) * y2 + pow(t,3) * y3);
        }
        

        // double tangentLineAngle(double t)
        // {
        //     double x0 = p0.x;
        //     double x1 = p1.x;
        //     double x2 = p2.x;
        //     double x3 = p3.x;
        //     double y0 = p0.y;
        //     double y1 = p1.y;
        //     double y2 = p2.y;
        //     double y3 = p3.y;

        //     ex = symbols('x')
        //     omt = 1-ex
        //     # bx = pow(omt,3) * x0 + 3 * pow(omt,2) * t * x1 + 3*omt * pow(t,2) * x2 + pow(t,3) * x3
        //     # by = pow(omt,3) * y0 + 3 * pow(omt,2) * t * y1 + 3*omt * pow(t,2) * y2 + pow(t,3) * y3

        //     bx = pow(omt,3) * x0 + 3 * pow(omt,2) * ex * x1 + 3*omt * pow(ex,2) * x2 + pow(ex,3) * x3
        //     by = pow(omt,3) * y0 + 3 * pow(omt,2) * ex * y1 + 3*omt * pow(ex,2) * y2 + pow(ex,3) * y3
            

        //     bpx = diff(bx,ex).evalf(subs={ex: t})
        //     bpy = diff(by,ex).evalf(subs={ex: t})

        //     m = bpy/bpx

        //     return(atan2(1,m))
        // }

        std::vector<coordinate> createLUT(double resolution)
        {
            std::vector<coordinate> points;
            // std::vector<double> angles;

            for (int i=0; i < resolution; i++)
            {
                points.push_back(solve(i/resolution));
                // angles.push_back(tangentLineAngle(i/resolution));
            }

            return (points);
        }

        double approximateLength(std::vector<coordinate> lut, double resolution)
        {
            double length = 0;

            for (int i=0; i < resolution; i++)
            {
                coordinate first = lut[i];
                coordinate second = lut[i+1];
                length += distToPoint(first,second);
            }

            return length;
        }
};

class util::pidConstants
{
    public:
        //decel and acel = volts/msec / msec
        double p,i,d,tolerance,integralThreshold, maxIntegral, kv;
        pidConstants(){}
        pidConstants(double kp, double ki, double kd, double tolerance, double integralThreshold, double maxIntegral) : p(kp), i(ki), d(kd), tolerance(tolerance), integralThreshold(integralThreshold), maxIntegral(maxIntegral) {}

        pidConstants(double kp, double ki, double kd, double tolerance, double integralThreshold, double maxIntegral, double kv) : p(kp), i(ki), d(kd), tolerance(tolerance), integralThreshold(integralThreshold), maxIntegral(maxIntegral), kv(kv) {}
};

class util::pid
{
    private:

        double prevError,error,derivative,integralThreshold;
        double integral = 0;
        util::pidConstants constants;

    public:
    
        pid(){}
        pid(util::pidConstants cons, double error) : constants(cons), prevError(error){}

        double out(double error)
        {
            //eyes
            // integral = std::fabs(error) <= constants.tolerance ? 0 : std::fabs(error) < integralThreshold ? integral + error : integral;
            
            if(std::fabs(error) < constants.tolerance) integral = 0;
            else if(std::fabs(error) < constants.integralThreshold) integral += error;
            if(integral > constants.maxIntegral) integral = constants.maxIntegral;

            //deex
            derivative = error - prevError;
            prevError = error;

            return(error * constants.p  + integral * constants.i + derivative * constants.d);
        }

        void update(util::pidConstants cons)
        {
            constants = cons;
        }
};

class util::movingAverage
{
    private:
        int size;
        double sum;
        double integral;
        std::vector<double> window;

    public:
        movingAverage(int Size) : size(Size)
        {
            for (int i = 0; i < size; i++)
            {
                window.push_back(0);
                integral += pow(i * 1.0/size * 1.0,2);
            }
        }
        
        void push(double val)
        {
            for (int i = 0; i < size-1; i++)
            {
                window[i] = window[i+1];
            }

            window[size - 1] = val;
            
        }

        double simpleAverage()
        {
            double average = 0;
            for(int i = 0; i < size; i++)
            {
                average += window[i];
            }
            
            return(average/size);
        }

        double expAverage()
        {
            double average = 0;
            for(int i = 1; i != size; i++)
            {
                average += window[i] * pow((i * 1.0)/(size * 1.0),2);
            }
            
            return(average/integral);
        }
};

class util::timeRange
{
    private:
        int start;
        int end;
    
    public:
        timeRange(int s, int e) : start(s), end(e) {}

        bool inRange(int time)
        {
            return (time >= start && time <= end);
        }

        int getStart()
        {
            return (start);
        }
};

class util::controller
{
    private:
        pros::Controller* cont;
        double leftCurve;
        double rightCurve;

    public:
        controller(pros::Controller& cont) : cont(&cont) {}

        enum driveMode
        {
            arcade,
            tank
        };

        int select(int num, std::vector<std::string> names)
        {
            int curr = 0;
            cont -> clear();
            while(1)
            {   
                if(cont -> get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT))
                {
                    if (curr != num-1)
                    {
                        curr++;
                    }
                    
                    else
                    {
                        curr = 0;
                    }
                }

                if(cont -> get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT))
                {
                    if (curr != 0)
                    {
                        curr--;
                    }

                    else
                    {
                        curr = num-1;
                    }
                }

                if(cont -> get_digital(pros::E_CONTROLLER_DIGITAL_A))
                {
                    pros::delay(200);
                    return(curr);
                }

                cont -> print(0, 0, "%s         ", names[curr]);
                pros::delay(50);
            }
        }
        
        std::vector<bool> getAll(std::vector<pros::controller_digital_e_t> buttons)
        {
            std::vector<bool> out;
            for (pros::controller_digital_e_t button : buttons)
            {
                out.push_back(cont -> get_digital(button));
                out.push_back(cont -> get_digital_new_press(button));
            }
            return(out);
        }

        //https://www.desmos.com/calculator/puepnlubzh
        double curve(double x, double scale) 
        {
            if (scale != 0) 
            {
                return(pow(2.718, (scale * ((std::fabs(x) - 127))) / 1000 ) * x); 
            }
            return x;
        }

        std::vector<double> drive(int direction, controller::driveMode mode)
        {   
            double lStick = curve(cont -> get_analog(ANALOG_LEFT_Y) * direction, leftCurve);
            double rStick;
            switch(mode)
            {
                case arcade:
                    rStick = curve(cont ->get_analog(ANALOG_RIGHT_X), rightCurve);
                    return(std::vector<double>{lStick + rStick, lStick - rStick});
                
                case tank:
                    rStick = curve(cont -> get_analog(ANALOG_RIGHT_Y), rightCurve);
                    return(std::vector<double>{lStick, rStick});
            }

            //you shoudlnt be here !
            return(std::vector<double>{0,0});
        }

        void setCurves(double left, double right)
        {
            leftCurve = left;
            rightCurve = right;
        }
};

// struct util::args
// {
//     double target;
//     bool hasArgs = 1;
//     util::pid pid;
// };

// struct util::action 
// {
//     void(*func)(util::args);
//     util::timeRange range;
//     util::args args;
// }; 

double util::dtr(double input) //NOLINT
{
  return(PI * input/180);
}

double util::rtd(double input) //NOLINT
{
  return(input * 180/PI);
}

int util::dirToSpin(double target,double currHeading) //NOLINT
{
    double d = (target - currHeading);
    double diff = d < 0 ? d + 360 : d;
    return(diff > 180 ? 1 : -1);
}

double util::minError(double target, double current)
{
    double b = std::max(target,current);
    double s = std::min(target,current);
    double diff = b - s;
    
    return(diff <= 180 ? diff : (360-b) + s);
}

double util::distToPoint(util::coordinate p1, util::coordinate p2) //NOLINT
{
    return( sqrt( pow((p2.x-p1.x),2) + pow((p2.y-p1.y), 2)));
}

double util::mod(double a, double b) //NOLINT
{
  return fmod(360-std::abs(a), b);
}

double util::absoluteAngleToPoint(util::coordinate pos, util::coordinate point) //NOLINT
{
    double t;

    try
    { 
        // t = -atan2(pos.y-point.y,pos.x - point.x) - PI/2;
        t = atan2(point.x - pos.x, point.y - pos.y);
    }

    catch(...)
    {
        t = PI/2;
    }
    
    t = util::rtd(t);

    // -270 - 90
    
    // if(t < -180)
    // {
    //     t = 90 + (270 - fabs(t));
    // }

    //-180 - 180

    t = -t;
    t = t >= 0 ? t :  180 + 180+t;
    return (t);
}

double util::imuToRad(double heading) //NOLINT
{
    // could be like shifted over? idk
    return (heading < 180) ? dtr(heading) : dtr(-(heading - 180));
}

double util::sign(double a)
{
    return(a > 0 ? 1 : -1);
}

#endif
