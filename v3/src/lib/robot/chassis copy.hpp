// #ifndef __CHASSIS__
// #define __CHASSIS__

// #include "groups.hpp"
// #include "sensors.hpp"
// #include "util/util.hpp"
// // #include <unordered_map>

// // namespace lib
// // {
// //     class chassis
// //     {
// //         private: 

// //           struct action 
// //           {
// //             double target;
// //             util::timeRange range;
// //             util::pidConstants constants;
// //           }; 

// //           lib::diffy chass;
// //           lib::imu imu;
// //           util::coordinate pos;
// //           double DL;
// //           double DR;

// //           std::vector<action> driveActions;
// //           std::unordered_map<int, util::pid> driveHash;

// //         public:
// //             chassis(lib::diffy mtrs, lib::imu inertial, util::coordinate position, double dl = 0, double dr = 0) : chass(mtrs), imu(inertial), pos(position), DL(dl), DR(dr) {}

// //             void drive(double target, util::pid pidController) 
// //             {
// //               chass.spin(pidController.out(target - chass.getRotation()));
// //             }

// //             void setDrive(double target, util::timeRange range, util::pidConstants constants)
// //             {
// //               driveActions.push_back({target, range, constants});
// //             }

// //             void initDrive()
// //             {
// //               for (action i : driveActions)
// //               {
// //                 driveHash[i.range.getStart()] = util::pid(i.constants, i.target);
// //               }
// //             }

// //             void driveStager(util::timer timer)
// //             {
// //               int time = timer.time();
// //               for (action i : driveActions)
// //               {
// //                 if(i.range.inRange(time))
// //                 {
// //                   drive(i.target, driveHash[i]);
// //                 }
// //               }
// //             }

// //             void 
// //     };
// // }



// namespace lib
// {
//   namespace chassis
//   {
//     void drive(util::args args) 
//     {
//       chass.spin(args.pid.out(args.target - chass.getRotation()));
//     }
//   }
// }
// #endif


