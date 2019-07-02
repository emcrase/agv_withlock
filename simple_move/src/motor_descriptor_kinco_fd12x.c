#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "../include/motor_descriptor.h"

#define DELAY_MS 200000

int kinco_fd12x_CAN_init(char *dev) // dependencies: TX2 platform socket CAN
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

void kinco_fd12x_CAN_SDO_send(int s, char node_id, const char *p_data)
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

#define SERVO_MODE_POSITION 0x01
#define SERVO_MODE_VELOCITY 0x03

void kinco_fd12x_servo_reinit(int fd, char node_id)
{
    char config0[8] = {0x2B, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    char config1[8] = {0x2B, 0x40, 0x60, 0x00, 0x86, 0x00, 0x00, 0x00};
    char config2[8] = {0x2B, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00};
    char config3[8] = {0x2F, 0x60, 0x60, 0x00, SERVO_MODE_VELOCITY, 0x00, 0x00, 0x00};
    
    kinco_fd12x_CAN_SDO_send(fd, node_id, config0);
    kinco_fd12x_CAN_SDO_send(fd, node_id, config1);
    kinco_fd12x_CAN_SDO_send(fd, node_id, config2);
    kinco_fd12x_CAN_SDO_send(fd, node_id, config3);
}

float kinco_fd12x_velocity_get(int fd, char node_id)
{
    return 0.0;
}

void kinco_fd12x_velocity_set(int fd, char node_id, char flag, float rpm)
{
    char sdo_str_mode[8] = {0x2F, 0x60, 0x60, 0x00, SERVO_MODE_VELOCITY, 0x00, 0x00, 0x00};
    
    if(flag) {
        kinco_fd12x_CAN_SDO_send(fd, node_id, sdo_str_mode);
    }
    
    char sdo_str[8] = {0x23, 0xFF, 0x60, 0x00};
    int *p_sdo_str_vel = (int *)(&(sdo_str[4]));

    rpm = rpm * 512.0 * 65535.0 / 1875.0;
    *p_sdo_str_vel = (int)rpm;

    kinco_fd12x_CAN_SDO_send(fd, node_id, sdo_str);
}

int kinco_fd12x_position_get(int fd, char node_id)
{
    return 0;
}

void kinco_fd12x_position_set(int fd, char node_id, char flag, int inc)
{}


