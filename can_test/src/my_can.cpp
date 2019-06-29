#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <cstring>

using namespace std;

#define DELAY_MS  				200000
#define MAX_MOTOR_NUM_SUPPORTED 		8

struct motor_descriptor {
    int fd;
    char node_id;
};

void CAN_SDO_send(int s, char node_id, const char *p_data)
{
    struct can_frame frame;
    int i;
    
    frame.can_id  = 0x600 + node_id;
    frame.can_dlc = 8;
    for(i=0; i<8; i++)
    {
        frame.data[i] = p_data[i];
    }
    
    write(s, &frame, sizeof(frame));
    
    usleep(DELAY_MS);
}

int CAN_init(char *dev)
{
    int s;
    int canfd_enable = 1;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_frame frame;

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, dev);
    ioctl(s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (const struct sockaddr *)(&addr), sizeof(addr));
    
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_enable, sizeof(canfd_enable));

    return s;
}

void __Servo_single_init(char node_id, int s)
{
    char config0[8] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    char config1[8] = {0x2B, 0x40, 0x60, 0x00, 0x86, 0x00, 0x00, 0x00};
    char config2[8] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00};
    char config3[8] = {0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00};
    
    CAN_SDO_send(s, node_id, config0);
    CAN_SDO_send(s, node_id, config1);
    CAN_SDO_send(s, node_id, config2);
    CAN_SDO_send(s, node_id, config3);
}

void Servo_all_init(int s)
{
    unsigned char i;
    unsigned char mt_num;
    struct motor_descriptor *mt_list[MAX_MOTOR_NUM_SUPPORTED];
    struct motor_descriptor wheel0, wheel1;

    wheel0.fd = s;
    wheel0.node_id = 1;

    wheel1.fd = s;
    wheel1.node_id = 0;

    mt_num = 2;
    mt_list[0] = &wheel0;
    mt_list[1] = &wheel1;
    
    for(i=0; i<mt_num; i++)
    {
        if((mt_list[i])->node_id == 0)
            break;
        
        __Servo_single_init((mt_list[i])->node_id, (mt_list[i])->fd);
    }
}

void velocity_set(int s, char node_id, float rpm)
{
    char sdo_str[8] = {0x23, 0xFF, 0x60, 0x00};
    int *p_sdo_str_vel = (int *)(&(sdo_str[4]));

    rpm = rpm * 512.0 * 65535.0 / 1875.0;
    *p_sdo_str_vel = (int)rpm;

    CAN_SDO_send(s, node_id, sdo_str);
}

int main(int argc, char *argv[])
{
    int can_socket;
    char dev[] = "can0";
    
    can_socket = CAN_init(dev);
    Servo_all_init(can_socket);

    float velocity;

    while(true) {
        cout<<"set motor velocity: ";
        cin>>velocity;

        velocity_set(can_socket, 1, velocity);
    }
    
    return 0;
}


