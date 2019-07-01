#define KINCO_FD12X_SUPPORTED

typedef struct md {
    int fd;
    char node_id;
    char mode;
    float velocity;
    int position;
    
    void (*servo_reinit)(motor_descriptor *md);
    void (*velocity_update)(motor_descriptor *md);
    void (*velocity_set)(motor_descriptor *md, float rpm);
    void (*position_update)(motor_descriptor *md);
    void (*position_set)(motor_descriptor *md, int inc);
} motor_descriptor;

#ifdef KINCO_FD12X_SUPPORTED
external motor_descriptor* kinco_fd12x_md_create(char *, char);
external void md_kinco_fd12x_destroy(motor_descriptor *);
#endif
