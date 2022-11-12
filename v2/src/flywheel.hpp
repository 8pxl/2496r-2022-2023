#ifndef __FLYWHEEL__
#define __FLYWHEEL__

#include "global.hpp"
#include "util.hpp"
#include <algorithm> 
#include <cmath>
#include <numeric>
#include <vector>

namespace flywheel
{
    double target;
    double gError;
    int ff;
    // bool inRange;
    
    double voltageOut(double kp, double kv, double ki, double integral, double target, double error, double deadband)
    {
        if (std::abs(error) < deadband)
        {
            // inRange = true;
            if(error > 0)
            {
                // if (std::abs(error) < 4)
                // {
                //     return (target + error * (kp/2) + integral * ki) * kv;
                // }

                return (target + error * kp + integral * ki) * kv;
            }

            else
            {
                // if (std::abs(error) < 4)
                // {
                //     return (target + error * (kp/2) + integral * ki) * kv;
                // }

                return (target + (error * 0.7) * kp + integral * ki) * kv;
            }
        }

        else if (error > 0)
        {
            // inRange = false;
            return 127;
        }

        else
        {
            // inRange = false;
            return (target/1.5) * kv;
        }
    }

    void spin()
    {
        const int velAverageSize = 30;
        const double kv = 0.1913474101312919;
        const double integralThreshold = 5;
        double ki;
        double kp;
        util::movingAverage velAverage = util::movingAverage(velAverageSize);
        util::timer forwardTimer;
        util::timer postForward;
        double speed;
        double error;
        double absError;
        double voltage;
        double integral;
        double deadband;
        // const int slidingWindowSize = 50;
        // double slidingWindow[slidingWindowSize] = {0};


        while (true)
        {

            //velocity sliding average
            velAverage.push(robot::flywheel.getSpeed());
            // speed = velAverage.simpleAverage();
            speed = velAverage.expAverage();

            // speed = 0;

            error = target - speed;
            absError = std::abs(error);
            gError = absError;

            if (target < 390)
            {
                kp = 4;
                ki = 0.07;
                deadband = 40;
            }

            else if (target < 490)
            {
                kp = 14;
                ki = 0.7;
                deadband = 50;
            }

            else if(target < 530)
            {
                kp = 6;
                ki = 0.01;
                deadband = 30;
            }

            else
            {
                kp = 14;
                ki = 0.09;
                deadband = 40;
            }


            if(absError < integralThreshold)
            {
                if(error > 0)
                {
                    integral += error;
                }

                else
                {
                    integral += error;
                }
            }

            else
            {
                integral = 0;
            }
            
            switch (ff)
            {
                case -1:

                    voltage = voltageOut(kp, kv, ki, integral, target, error, deadband);
                    forwardTimer.start();
                    break;

                case 0:
                    voltage = 127;
                    break;

                case 1:
                    if(forwardTimer.time() >= 0)
                    {
                        voltage = 127;
                        // deadband = ;

                        if(forwardTimer.time() >= 500)
                        {
                            robot::flywheel.stop("c");
                            // for (int i = 0; i < 7; i ++)
                            // {
                            //     printf("%f,%f,", speed, 0.0);
                            // }
                            pros::delay(180);
                            // robot::flywheel.spin(127);
                            // pros::delay(200);
                            ff = -1;
                        }
                    }
                    break;
                
                case 3:
                    if(forwardTimer.time() >= 60)
                    {
                        if(forwardTimer.time() >= 600)
                        {
                            ff = -1;
                        }

                        else
                        {
                            voltage = 127;
                        }
                    }
                    break;
            }

            // glb::controller.print(1, 1, "%f", voltage);


            // else
            // {
            //     forwardTimer.start();
            //     voltage = voltageOut(kp, kv, ki, integral, target, error, deadband);
            // }

            // if(ff)
            // {   
            //     deadband = 4;
                // if(forwardTimer.time() >= 50)
                // {
                //     voltage = 127;

                //     if(forwardTimer.time() <= 800)
                //     {
                //         ff = false;
                //     }
                // }
            // }

            // voltage = voltageOut(kp, kv, ki, integral, target, error, deadband);
            robot::flywheel.spin(voltage);
            
            pros::delay(10);
            // printf("%f,", speed);
            // if (ff != -1)
            // {
            //     printf("%f,", speed/2);
            // }

            // else
            // {
            //     printf("%f,%f,", speed,voltage);
            // }

            // printf("%f,%f,", speed,integral);
        }
    }
}



            // //rotate array
            // for (int i = 1; i < slidingWindowSize-1; i++)
            // {
            //     slidingWindow[i] = slidingWindow[i+1];
            // }

            // slidingWindow[slidingWindowSize - 1] = currSpeed;
            
            // //sum values
            // for(int i = 0; i < slidingWindowSize; i++)
            // {
            //     speed += slidingWindow[i];
            // }
            
            // speed = speed/slidingWindowSize;

            // velAverage.update(robot::flywheel.getSpeed());
            // speed = velAverage.average();
            

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


    // void spin()
    // {

    //     double voltageCommands;
    //     const int slidingWindowSize = 50;
    //     const int errorWindowSize = 50;
    //     double slidingWindow[slidingWindowSize] = {0};
    //     double errorWindow[errorWindowSize] = {0};
    //     // double steadyStateError = -27;
    //     double steadyStateError = 0;
    //     double error = 0;
    //     double prevError = 0;
    //     double derivative;
    //     double kd = 2;
    //     // double kd = 0;
    //     double derivativeSummation = 0;

    //     while (true)
    //     {
    //         if(glb::match)
    //         {
    //             if(!glb::driver)
    //             {
    //                 return;
    //             }
    //         }
            
    //         //velocity sliding average
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

    //         //error sliding average
    //         error = target - averageSpeed;
    //         // double averageError = 0;
            
    //         // //rotate array
    //         // for (int i = 1; i < errorWindowSize-1; i++)
    //         // {
    //         //     errorWindow[i] = errorWindow[i+1];
    //         // }

    //         // errorWindow[errorWindowSize - 1] = error;

    //         // //sum values
    //         // for(int i = 0; i < errorWindowSize; i++)
    //         // {
    //         //     averageError += errorWindow[i];
    //         // }

    //         // averageError = averageError/errorWindowSize;

    //         derivative = error - prevError;
    //         prevError = error;
            
    //         // steadyStateError = std::abs(error) * steadyStateScale;

    //         // robot::flywheel.spin((target + steadyStateError)* 127/600 );

    //         if (derivative > 0.5)
    //         {
    //             derivative = 0;
    //         }

    //         double correctionTerms = (steadyStateError * 127/600) + (kd * derivative);

    //         if (std::abs(error) < 30)
    //         {
    //             robot::flywheel.spin((averageSpeed + error/1.5) * 127 / 600 + correctionTerms);
    //             // voltageCommands = (averageSpeed + averageError/1.5) * 127 / 600 + correctionTerms;
    //         }

    //         if(std::abs(error) < 50)
    //         {
    //             robot::flywheel.spin((target * 127/600) + error/0.8);
    //         }

    //         else
    //         {   
    //             double voltage = target * 127 / 600;    

    //             if (target > averageSpeed)
    //             {
    //                 robot::flywheel.spin(voltage * 2 + correctionTerms);
    //                 // voltageCommands = voltage * 2 + correctionTerms;
    //             }

    //             else 
    //             {
    //                 robot::flywheel.spin(voltage/1.5 + correctionTerms);
    //                 // voltageCommands = (voltage/1.5 + correctionTerms);
    //             }
    //         }

    //         printf("%f,", averageSpeed);
    //         // glb::controller.print(0, 0, "%f", currSpeed);
    //         pros::delay(10);
    //         // glb::controller.print(3, 0, "%f", target);
    //     }
    // }

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
            // printf("%f,", averageSpeed);
    //     }
    // }

#endif