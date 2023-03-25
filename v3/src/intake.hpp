#include "global.hpp"

namespace intake
{
    enum states {intaking, idling, awaiting};
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
                    robot::itsuki.spin(speed);
                    break;

                case idling:
                    robot::itsuki.stop('c');
                    break;
                
                case awaiting:
                    if(timer.time() > delay)
                    {
                        curr = next;
                    }
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
}