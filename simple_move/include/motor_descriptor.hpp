using namespace motor_descriptor;

#define KINCO_FD12X_SUPPORTED

extern "C" int kinco_fd12x_CAN_init(char *);
extern "C" void kinco_fd12x_servo_reinit(int, char);
extern "C" float kinco_fd12x_velocity_get(int, char);
extern "C" void kinco_fd12x_velocity_set(int, char, char, float);
extern "C" int kinco_fd12x_position_get(int, char);
extern "C" void kinco_fd12x_position_set(int, char, char, int);

enum servo_model_type {
    SERVO_MODEL_KINCO_FD12X = 0x1
};

enum servo_mode {
    SERVO_MODE_POSITION = 1,
    SERVO_MODE_VELOCITY = 2
};

class motor_descriptor {
public:
    motor_descriptor(char *dev_name, char node_id)
        : node_id(node_id), mode(SERVO_MODE_VELOCITY),
          velocity(0.0), position(0.0)
    {
        fd = kinco_fd12x_CAN_init(dev_name);
    }
    
    virtual void servo_reinit() {}
    virtual void velocity_update() {}
    virtual void velocity_set(float rpm) {}
    virtual void position_update() {}
    virtual void position_set(int inc) {}
    
    int fd;
    char node_id;
    char mode;
    float velocity;
    int position;
};

class md_kinco_fd12x : public motor_descriptor {
    void servo_reinit() {
        kinco_fd12x_servo_reinit(fd, node_id);
    }
    
    void velocity_update() {
        velocity = kinco_fd12x_velocity_get(fd, node_id);
    }
    
    void velocity_set(float rpm) {
        char flag = 0;
        if(mode != SERVO_MODE_VELOCITY) flag = 1;
        
        kinco_fd12x_velocity_set(fd, node_id, flag, rpm);
    }
    
    void position_update() {
        position = kinco_fd12x_position_get(fd, node_id);
    }
    
    void position_set(int inc) {
        char flag = 0;
        if(mode != SERVO_MODE_POSITION) flag = 1;
        
        kinco_fd12x_position_set(fd, node_id, flag, inc);
    }
}

