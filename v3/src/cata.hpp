#ifndef __CATA__
#define __CATA__
#include "global.hpp"

namespace cata
{
    enum states {idle, firing, reloading, paused};
    states curr;
    bool boost;
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
                    break;

                case firing:
                    boost ? robot::boost.setState(true) : robot::boost.setState(false);
                    if(limit)
                    {
                        robot::itsuki.spin(-127);
                    }

                    else
                    {
                        curr = reloading;
                        robot::boost.setState(false);
                    }
                    break;
                
                case reloading:
                    if(!limit)
                    {
                        robot::itsuki.spin(-127);
                    }

                    else
                    {
                        robot::itsuki.stop('b');
                        curr = idle;
                    }
                    break;

                case paused:
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