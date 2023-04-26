#ifndef __INTAKE__
#define __INTAKE__
#include "global.hpp"

namespace intake
{
    enum states {intaking, idling, awaiting, pistonUp, pistonDown};
    states curr;
    pros::Mutex smtx;
    int speed;
    int delay;
    states next;
    util::timer timer;

    void control()
    {
        while(true)
        {
            smtx.take();

            switch(curr)
            {
                case intaking:
                    if(glb::limit.get_value())
                    {
                        robot::itsuki.spin(speed);
                    }
                    break;

                case idling:
                    break;
                
                case awaiting:
                    robot::itsuki.spin(127);
                    if(timer.time() > delay)
                    {
                        robot::itsuki.stop('c');
                        curr = next;
                    }
                    break;

                case pistonUp:
                    robot::tsukasa.setState(true);
                    curr = intaking;
                    break;

                case pistonDown:
                    robot::tsukasa.setState(false);
                    curr = intaking;
                    break;
            }

            smtx.give();
            pros::delay(20);
        }
    }

    void spin(int speed)
    {
        smtx.take();
        intake::speed = speed;
        curr = intaking;
        smtx.give();
    }

    void stop()
    {
        smtx.take();
        robot::itsuki.stop('c');
        curr = idling;
        smtx.give();
    }

    void asyncAction(states state, int delay, int speed = intake::speed)
    {
        smtx.take();
        intake::speed = speed;
        intake::delay = delay;
        intake::next = state;
        intake::timer.start();
        curr = awaiting;
        smtx.give();
    }

    void asyncPiston(int delay)
    {
        smtx.take();
        intake::delay = delay;
        intake::next = intake::pistonDown;
        intake::timer.start();
        curr = awaiting;
        smtx.give();
    }
}

#endif