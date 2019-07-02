#define KINCO_FD12X_SUPPORTED

typedef struct _md {
    int fd;
    char node_id;
    char mode;
    float velocity;
    int position;
    
    void (*servo_reinit)(struct _md *md);
    void (*velocity_update)(struct _md *md);
    void (*velocity_set)(struct _md *md, float rpm);
    void (*position_update)(struct _md *md);
    void (*position_set)(struct _md *md, int inc);
} motor_descriptor;

#ifdef KINCO_FD12X_SUPPORTED
extern motor_descriptor* kinco_fd12x_md_create(char *, char);
extern void md_kinco_fd12x_destroy(motor_descriptor *);
#endif
