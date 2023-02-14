#include "util/util.hpp"
#include <unordered_map>

namespace lib
{
    class stager
    {
        private:
          std::vector<util::action> actions;

        public:
            stager(std::vector<util::action> a) : actions(a) {}

            void run(util::timer timer)
            {
                while (true)
                {
                    int time = timer.time();
                    for (util::action i : actions)
                    {
                        if(i.range.inRange(time))
                        {
                            i.func(i.args);
                        }
                    }
                }
            }
    };
    
}