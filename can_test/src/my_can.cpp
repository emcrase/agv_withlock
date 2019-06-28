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

#define DELAY_MS  200000

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

void CAN_init(int *s)
{
    int canfd_enable = 1;
    struct ifreq ifr;
    struct sockaddr_can addr;
    struct can_frame frame;

    *s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0");
    ioctl(*s, SIOCGIFINDEX, &ifr);
    
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(*s, (const struct sockaddr *)(&addr), sizeof(addr));
    
    setsockopt(*s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    setsockopt(*s, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_enable, sizeof(canfd_enable));
}

void __Servo_single_init(char node_id, int s)
{
    char config0[8] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    char config1[8] = {0x2B, 0x40, 0x60, 0x00, 0x86, 0x00, 0x00, 0x00};
    char config2[8] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00};
    char config3[8] = {0x2F, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00};
    char config4[8] = {0x23, 0xFF, 0x60, 0x00, 0x00, 0x00, 0x40, 0x00};
    
    CAN_SDO_send(s, node_id, config0);
    CAN_SDO_send(s, node_id, config1);
    CAN_SDO_send(s, node_id, config2);
    CAN_SDO_send(s, node_id, config3);
    CAN_SDO_send(s, node_id, config4);
}

void Servo_all_init(int s)
{
    char node_id_array[6] = {1, 0, 0, 0, 0, 0};
    int i;
    
    for(i=0; i<sizeof(node_id_array); i++)
    {
        if(node_id_array[i] == 0)
            break;
        
        __Servo_single_init(node_id_array[i], s);
    }
}

int main(int argc, char *argv[])
{
    int can_socket;
    
    CAN_init(&can_socket);
    Servo_all_init(can_socket);
    
    return 0;
}


