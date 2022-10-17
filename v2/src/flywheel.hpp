#ifndef __FLYWHEEL__
#define __FLYWHEEL__

#include "global.hpp"
#include "pros/rtos.hpp"
#include "util.hpp"
#include <algorithm> 
#include <numeric>
#include <vector>

namespace flywheel
{
    double target;
    
    // void spin()
    // {
    //     while (true)
    //     {
    //         double currSpeed = robot::flywheel.getSpeed();
    //         double diff = target - currSpeed;
            
    //         // steadyStateError = std::abs(diff) * steadyStateScale;
    //         steadyStateError = 0;

    //         if (std::abs(diff) < 30)
    //         {
    //             robot::flywheel.spin((currSpeed + diff/2) * 127 / 600 + steadyStateError);

    //             if (target > 350)
    //             {
    //                 glb::controller.rumble("-");
    //             }
    //         }

    //         else
    //         {
    //             double voltage = target * 127 / 600;

    //             if (target > currSpeed)
    //             {
    //                 robot::flywheel.spin(voltage * 2 + steadyStateError);
    //             }

    //             else 
    //             {
    //                 robot::flywheel.spin(voltage + steadyStateError);
    //             }
    //         }
    //         // glb::controller.print(0, 0, "%f", currSpeed);
    //         // pros::delay(50);
    //         // glb::controller.print(3, 0, "%f", target);
    //     }
    // }


    void spin()
    {

        double voltageCommands;
        const int slidingWindowSize = 50;
        const int errorWindowSize = 50;
        double slidingWindow[slidingWindowSize] = {0};
        double errorWindow[errorWindowSize] = {0};
        // double steadyStateError = -27;
        double steadyStateError = 0;
        double error = 0;
        double prevError = 0;
        double derivative;
        double kd = 2;
        // double kd = 0;
        double derivativeSummation = 0;

        while (true)
        {
            if(glb::match)
            {
                if(!glb::driver)
                {
                    return;
                }
            }
            
            //velocity sliding average
            double averageSpeed = 0;
            double currSpeed = robot::flywheel.getSpeed();

            //rotate array
            for (int i = 1; i < slidingWindowSize-1; i++)
            {
                slidingWindow[i] = slidingWindow[i+1];
            }

            slidingWindow[slidingWindowSize - 1] = currSpeed;
            
            //sum values
            for(int i = 0; i < slidingWindowSize; i++)
            {
                averageSpeed += slidingWindow[i];
            }
            
            averageSpeed = averageSpeed/slidingWindowSize;

            //error sliding average
            error = target - averageSpeed;
            // double averageError = 0;
            
            // //rotate array
            // for (int i = 1; i < errorWindowSize-1; i++)
            // {
            //     errorWindow[i] = errorWindow[i+1];
            // }

            // errorWindow[errorWindowSize - 1] = error;

            // //sum values
            // for(int i = 0; i < errorWindowSize; i++)
            // {
            //     averageError += errorWindow[i];
            // }

            // averageError = averageError/errorWindowSize;

            derivative = error - prevError;
            prevError = error;
            
            // steadyStateError = std::abs(error) * steadyStateScale;

            // robot::flywheel.spin((target + steadyStateError)* 127/600 );

            if (derivative > 0.5)
            {
                derivative = 0;
            }

            double correctionTerms = (steadyStateError * 127/600) + (kd * derivative);

            if (std::abs(error) < 30)
            {
                robot::flywheel.spin((averageSpeed + error/1.5) * 127 / 600 + correctionTerms);
                // voltageCommands = (averageSpeed + averageError/1.5) * 127 / 600 + correctionTerms;
            }

            if(std::abs(error) < 50)
            {
                robot::flywheel.spin((target * 127/600) + error/0.8);
            }

            else
            {   
                double voltage = target * 127 / 600;    

                if (target > averageSpeed)
                {
                    robot::flywheel.spin(voltage * 2 + correctionTerms);
                    // voltageCommands = voltage * 2 + correctionTerms;
                }

                else 
                {
                    robot::flywheel.spin(voltage/1.5 + correctionTerms);
                    // voltageCommands = (voltage/1.5 + correctionTerms);
                }
            }

            printf("%f,", averageSpeed);
            // glb::controller.print(0, 0, "%f", currSpeed);
            pros::delay(10);
            // glb::controller.print(3, 0, "%f", target);
        }
    }

    // void spin()
    // {
    //     const int slidingWindowSize = 50;
    //     const int errorWindowSize = 20;
    //     double slidingWindow[slidingWindowSize] = {0};
    //     int count = 0;
    //     int i = 1;
    //     while(1)
    //     {
    //         double averageSpeed = 0;
    //         double currSpeed = robot::flywheel.getSpeed();

    //         //rotate array
    //         for (int i = 1; i < slidingWindowSize-1; i++)
    //         {
    //             slidingWindow[i] = slidingWindow[i+1];
    //         }

    //         slidingWindow[slidingWindowSize - 1] = currSpeed;
            
    //         //sum values
    //         for(int i = 0; i < slidingWindowSize; i++)
    //         {
    //             averageSpeed += slidingWindow[i];
    //         }
            
    //         averageSpeed = averageSpeed/slidingWindowSize;
            
    //         if(count % 1000 == 0)
    //         {
    //             i++;
    //         }

    //         robot::flywheel.spin((50*i) * 127/600);

    //         pros::delay(10);
    //         count++;
    //         printf("%f,", averageSpeed);
    //     }
    // }

}

#endif