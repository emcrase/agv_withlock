#include <iostream>
#include "../include/motor_descriptor.h"

int main(int argc, char *argv[])
{
    motor_descriptor *left_wheel;
    float velocity;
    
    left_wheel = kinco_fd12x_md_create("can0", 1);
    left_wheel->servo_reinit(left_wheel);
    
    while(true) {
        cout<<"set motor velocity: ";
        cin>>velocity;
        
        if(velocity > 120.0) {
            cout<<"velocity beyond maxium.. exit"<<endl;
            break;
        }

        left_wheel->velocity_set(left_wheel, velocity);
    }
    
    return 0;
}