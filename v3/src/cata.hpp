#include "global.hpp"
#include <mutex>

namespace cata
{
    enum states {idle, firing, reloading, paused};
    states curr;
    pros::Mutex smtx;

    void control()
    {
        while(true)
        {
            smtx.take();
            bool limit = glb::limit.get_value();

            switch(curr)
            {
                case idle:
                    break;

                case firing:
                    if(limit)
                    {
                        robot::itsuki.spin(-127);
                    }

                    else
                    {
                        curr = reloading;
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
                    // pausing should be called in a loop
                    curr = reloading;
                    break;
            }

            smtx.give();
            pros::delay(20);
        }
    }

    void fire()
    {
        smtx.take();
        curr = firing;
        smtx.give();
    }  

    void pause()
    {
        smtx.take();
        curr = paused;
        smtx.give();
    }
} 
