// #include "vex.h"

// float pi = 3.14159;
// int x=0;
// int y=0;

// float dtr(float input){
//   return(pi/180 * input);
// }

// void odom(){
//   double prevRotation = imu.heading();
//   backLeft.setPosition(0,deg);
//   vex::task::sleep(200);

//   while(1){
//     double currRotation = imu.heading();

//     // calcualting change in rotation
//     double deltaRotation = currRotation - prevRotation;
//     prevRotation =  currRotation;
    
//     //change in encoder value
//     double deltaVert = backLeft.position(degrees);

//     // calculating change in global y

//     double sOverTheta = deltaVert/dtr(deltaRotation);

//     double deltaY = sin(dtr(deltaRotation)) * sOverTheta;

//     // calculating change in global x
//     sOverTheta = deltaVert/dtr(deltaRotation);
//     double deltaX = (1-cos(dtr(deltaRotation))) * sOverTheta;

//     // updating global x and global y
//     x = x + deltaX;
//     y = y + deltaY;

//     horizEncoder.setPosition(0,deg);
//     vertEncoder.setPosition(0,deg);

//     // printf("(%i,%i)\n",x,y);

//     vex::task::sleep(20);
//   }
// }

// double distToPoint(int px, int py){
//   return sqrt( (px-x)^2 + (py-y)^2 );
// }

// double absoluteAngleToPoint(int px, int py){
//   return 90 - imu.heading();
// }
