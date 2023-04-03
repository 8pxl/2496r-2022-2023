#ifndef __CATA__
#define __CATA__
#include "global.hpp"

namespace cata
{
    enum states {idle, firing, reloading, paused};
    states curr;
    pros::Mutex smtx;

    void control()
    {
        while(true)
        {
            smtx.lock();
            bool limit = glb::limit.get_value();

            switch(curr)
            {
                case idle:
                    std::cout << "cata idle!" << std::endl;
                    break;

                case firing:
                    std::cout << "cata firing! limit: " << limit << std::endl;
                    if(!limit)
                    {
                        robot::itsuki.spin(-30);
                    }

                    else
                    {
                        curr = reloading;
                    }
                    break;
                
                case reloading:
                    std::cout << "cata reloading! liimt: " << limit << std::endl;
                    if(limit)
                    {
                        robot::itsuki.spin(-30);
                    }

                    else
                    {
                        robot::itsuki.stop('b');
                        curr = idle;
                    }
                    break;

                case paused:
                    // pausing should be called in a loop
                    std::cout << "cata paused!" << std::endl;
                    robot::itsuki.stop('b');
                    curr = reloading;
                    break;
            }

            smtx.unlock();
            pros::delay(20);
        }
    }

    void fire()
    {
        smtx.lock();
        curr = firing;
        smtx.unlock();
    }  

    void pause()
    {
        smtx.lock();
        curr = paused;
        smtx.unlock();
    }
} 

#endif