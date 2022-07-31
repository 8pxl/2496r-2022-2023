#include "main.h"
#include "pros/rtos.hpp"
# define PI 3.14159265358979323846

namespace util
{
    class timer;
    double dtr(double input);
    double rtd(double input);
    int dirToSpin(double target,double currHeading);
    double minError(double target, double current);
}

class util::timer{
    public:
        int startTime = 0;

        void start()
        {
            startTime = pros::millis();
        }

        int time()
        {
            return (pros::millis() - startTime);
        }
};

double util::dtr(double input)
{
  return(PI * input/180);
}

double util::rtd(double input)
{
  return(input * 180/PI);
}

int util::dirToSpin(double target,double currHeading)
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