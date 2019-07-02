#include <iostream>
#include "../include/motor_descriptor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    md_kinco_fd12x left_wheel("can0", 1);
    float velocity;
    
    left_wheel.servo_reinit();
    
    while(true) {
        cout<<"set motor velocity: ";
        cin>>velocity;
        
        if(velocity > 120.0) {
            cout<<"velocity beyond maxium.. exit"<<endl;
            break;
        }

        left_wheel.velocity_set(velocity);
    }
    
    return 0;
}