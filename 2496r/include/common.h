#include <iostream>

#define PI 3.14159

extern int x;
extern int y;

double dtr(double input);
double mod(double a, double b);
double rtd(double input);
int sign(double input);
double distToPoint(int x, int y, int px, int py);
double absoluteAngleToPoint(int px, int py);
int dirToSpin(double target, double unscaled);

class coordinate{
    friend std::ostream& operator<<(std::ostream&, coordinate const&);
    private:
    
    int x;
    int y;
    
    public:
    
    coordinate(int a, int b){
        x = a;
        y = b;
    }
    
    int getX(){
        return x;
    }
    int getY(){
        return y;
    }
    
};
