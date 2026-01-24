#ifndef HW_MECHDOG_H
#define HW_MECHDOG_H
#include "quad_kinematics.h"
#include "pwm_servo.h"
#include "mech_base_types.h"
#include "Hiwonder.h"
#include "WMMatrixLed.h"
#include <MPU6050.h>

void read_angle(void);

extern int16_t ax, ay, az;
extern int16_t gx, gy, gz;
extern float ax0, ay0, az0;
extern float gx0, gy0, gz0;
extern float ax1, ay1, az1;
extern float gx1, gy1, gz1;

extern float dx;
extern float dz;
extern int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
extern float radianX;
extern float radianY;
extern float radianZ;
extern float radianX_last; //最终获得的X轴倾角
extern float radianY_last; //最终获得的Y轴倾角

class MechDog:public quad_kinematics, public PowerBuzzer{
  private:
    
    mech_point_t* pose;
    int pose_clock = 0;
    int pwm_clock = 0;

    int8_t state = 0;
    int8_t stop_flag = 1;
    int8_t action_run_state = 0;
    int8_t open_imu = 0;

    char action_name[20];

    typedef void (MechDog::*ActionFunc)();
    struct Action{
      const char* name;
      ActionFunc func;
    };
    
    static const Action actionList[];
    friend void timer_clock_callback(void *arg);
    void act_run_func();
    
  public:
    void MechDog_init(mech_point_t* pose = nullptr, int duration = 1000);
    void normal_attitude();
    void rotation_pitch();
    void rotation_roll();
    void set_pose(mech_pose_t p, int duration);
    void transform(mech_pose_t p, int duration);
    void set_default_pose(mech_point_t* pose = nullptr, int duration = 1000);
    void move(float speed_x, float angle_rate);
    
    int read_servo(int8_t id);
    void read_all_servo(int8_t *position);
    void set_servo(int8_t id, int position, int duration = 500);
    int read_offset(int8_t id);
    int read_angleoffset();
    void set_angleoffset(int8_t offset);
    void read_all_offset(int8_t *offset);
    void setsave_all_offset(int8_t *offset);
    void set_offset(int8_t id, int offset);
    void save_offset();
    void action_run(const char* action_name);
    int run_status();
    void homeostasis(bool onoff);
    bool read_homeostasis_status();
    static void balancing_Task(void *p);
    
};

#endif