// author@ likunyao
// date@ 2019/6/28
// description@ simple test case
#include <iostream>
#include "../include/motor_descriptor.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    const char dev_name[8] = "can0";
    md_kinco_fd12x left_wheel(dev_name, 1);
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
