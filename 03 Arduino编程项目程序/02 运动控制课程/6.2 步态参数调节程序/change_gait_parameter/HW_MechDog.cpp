#include "Wire.h"
#include "esp32-hal.h"
#include "HardwareSerial.h"
#include "HW_MechDog.h"
#include "action.h"
#include "pwm_servo.h"

#define MPU6050_ADDRESS 0x68
MPU6050 accelgyro(MPU6050_ADDRESS,&IIC1);
int16_t ax, ay, az;
int16_t gx, gy, gz;
float ax0, ay0, az0;
float gx0, gy0, gz0;
float ax1, ay1, az1;
float gx1, gy1, gz1;

float dx;
float dz;
int ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
float radianX;
float radianY;
float radianZ;
float radianX_last; //最终获得的X轴倾角
float radianY_last; //最终获得的Y轴倾角

mech_point_t default_pose[4] = {
  {59.25, 46.0, -80}, {-71.25, 46.0, -80}, {59.25, -46.0, -80}, {-71.25, -46.0, -80}
};

const MechDog::Action MechDog::actionList[] = {
    {"normal_attitude", &MechDog::normal_attitude},
    {"rotation_pitch", &MechDog::rotation_pitch},
    {"rotation_roll", &MechDog::rotation_roll},
};

IRAM_ATTR void timer_clock_callback(void *arg) {
  MechDog *dog = (MechDog *)arg;
  if(dog->pose_clock > 0) dog->pose_clock -= 50;
  else if(dog->pwm_clock > 0) dog->pwm_clock -= 50;
  else if(dog->stop_flag == 0) dog->state = 1;
  else dog->state = 0;
}

void MechDog::MechDog_init(mech_point_t* pose, int duration){
  Buzzer_init();
  pwm_servo_init();
  begin(pwm_servo_set_position);
  delay(1000);

  if(pose == nullptr) this->pose = default_pose;
  else this->pose = pose; 

  quad_kinematics::set_default_pose(this->pose, duration);
  set_gait_params(150, 200, 25);
  delay(duration);

  esp_timer_create_args_t timer_args = {
      .callback = &timer_clock_callback, // timer callback function
      .arg = this,
      .name = "clock_timer",
  };
  esp_timer_handle_t timer_handle;
  ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer_handle));
  ESP_ERROR_CHECK(esp_timer_start_periodic(timer_handle, 50000));
}

int MechDog::run_status(){
  return state;
}

/* motion status:1 */
void MechDog::set_default_pose(mech_point_t* pose, int duration){
  if(state != 2){
    state = 1;
    if(pose == nullptr) quad_kinematics::set_default_pose(default_pose, duration);
    else quad_kinematics::set_default_pose(pose, duration);
    pose_clock = duration < 30000 ? duration : 30000;
  }
}

/* action example */
void MechDog::normal_attitude(){
  mech_pose_t normal = {
    {0,0,0},{0,0,0}
  };
  set_pose(normal, 700);
}

void MechDog::rotation_roll(){
  mech_pose_t roll[3] = {
    {{0,0,0},{20,0,0}},
    {{0,0,0},{-20,0,0}},
    {{0,0,0},{0,0,0}}
  };
  state = 1;
  pose_clock = 4000;
  set_default_pose(default_pose, 300);
  delay(300);
  set_pose(roll[0], 400);
  delay(400);
  set_pose(roll[1], 800);
  delay(800);
  set_pose(roll[2], 400);
}

void MechDog::rotation_pitch(){
  mech_pose_t pitch[3] = {
    {{0,0,0},{0,20,0}},
    {{0,0,0},{0,-20,0}},
    {{0,0,0},{0,0,0}}
  };
  state = 1;
  pose_clock = 4000;
  set_default_pose(default_pose, 300);
  delay(300);
  set_pose(pitch[0], 400);
  delay(400);
  set_pose(pitch[1], 800);
  delay(800);
  set_pose(pitch[2], 400);
}

void MechDog::set_pose(mech_pose_t p,int duration){
  if(state != 2){
    mech_pose_t result_pose;
    result_pose.position.x = p.position.x;
    result_pose.position.y = p.position.y;
    result_pose.position.z = p.position.z;

    result_pose.orientation.roll = p.orientation.roll * PI / 180.0f;
    result_pose.orientation.pitch = p.orientation.pitch * PI / 180.0f;
    result_pose.orientation.yaw = p.orientation.yaw * PI / 180.0f;
    quad_kinematics::set_pose(result_pose, duration);
    pose_clock = duration < 30000 ? duration : 30000;
    state = 1;
  }
}

void MechDog::transform(mech_pose_t p,int duration){
  if(state != 2){
    mech_pose_t result_pose;
    result_pose.position.x = p.position.x;
    result_pose.position.y = p.position.y;
    result_pose.position.z = p.position.z;

    result_pose.orientation.roll = p.orientation.roll * PI / 180.0f;
    result_pose.orientation.pitch = p.orientation.pitch * PI / 180.0f;
    result_pose.orientation.yaw = p.orientation.yaw * PI / 180.0f;

    quad_kinematics::transform(result_pose, duration);
    pose_clock = duration < 30000 ? duration : 30000;
    state = 1;
  }
}

void MechDog::move(float speed_x, float angle_rate){
  if(state != 2){
    state = 1;
    quad_kinematics::move(speed_x, angle_rate* PI / 180.0f);
    if(speed_x == 0 && angle_rate == 0) stop_flag = 1;
    else stop_flag = 0;
  }
}

/* PWM Servos control status : 2 */
int MechDog::read_servo(int8_t id){
  return pwm_servo_position(id);
}

void MechDog::read_all_servo(int8_t *position){
  for(int i = 0;i < 8; i++){
    position[i] = pwm_servo_position(i+1);
  }
}

void MechDog::set_servo(int8_t id, int position,int duration){
  if(state != 1){
    state = 2;
    pwm_servo_position(id,position,duration);
    pwm_clock = duration < 30000 ? duration : 30000;
  }
}

int MechDog::read_offset(int8_t id){
  return get_pwm_servo_offset(id);
}

int MechDog::read_angleoffset(){
  return move_angle_read();
}

void MechDog::set_angleoffset(int8_t offset){
  move_angle_write(offset);
}

void MechDog::read_all_offset(int8_t *offset){
  offset[0] = read_angleoffset();

  for(int i = 1;i < 9; i++){
    offset[i] = get_pwm_servo_offset(i);
  }
}

void MechDog::setsave_all_offset(int8_t *offset){
  set_angleoffset(offset[0]);
  for(int i = 1; i < 9; i++){
    set_offset(i,offset[i]);
  }
  save_offset();
}

void MechDog::set_offset(int8_t id, int offset){
  if(state != 1){
    state = 2;
    set_pwm_servo_offset(id,offset);
  }
}

void MechDog::save_offset(){
  for(int i = 0; i < 8; i++){
    pwm_servo_save_offset(i+1);
  }
  move_angle_save();
}

/* action */
void MechDog::action_run(const char* action_name){
  if (run_status() == 0){
    strcpy(this->action_name, action_name);
    act_run_func();
  }
}

void MechDog::act_run_func(){
  action_run_state = 1;
  for(int i = 0; i<sizeof(actionList)/sizeof(actionList[0]); i++){
    if(strcmp(action_name,actionList[i].name) == 0){
      (this->*(actionList[i].func))();
      action_run_state = 0;
      return;
    }
  }
  for(int i = 0; i<sizeof(aclist)/sizeof(aclist[0]); i++){
    if(strcmp(action_name, aclist[i].name) == 0){
      mech_action* actions = aclist[i].actions;
      for(int j = 0; j<aclist[i].actionCount; j++){
        for(int k = 1; k<9; k++){
          set_servo(k,actions[j].Servos[k-1],actions[j].Duration);
        }
        delay(actions[j].Duration);
      }
    }
  }
  action_run_state = 0;
}

void MechDog::homeostasis(bool onoff){
  if(onoff){
    open_imu = 1;
    IIC1.begin(SDA1,SCL1);
    accelgyro.initialize();
    accelgyro.setFullScaleGyroRange(3); //设定角速度量程
    accelgyro.setFullScaleAccelRange(1); //设定加速度量程
    delay(200);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  //获取当前各轴数据以校准
    ax_offset = ax;  
    ay_offset = ay;  
    az_offset = az - 8192;  
    gx_offset = gx; 
    gy_offset = gy; 
    gz_offset = gz; 

    xTaskCreate(          
      balancing_Task,          
      "balancingTask",      
      2048,           
      this,           
      1,              
      NULL    
    );

  }else{
    open_imu = 0;
  }
}

bool MechDog::read_homeostasis_status(){
  if(open_imu == 0){
    return false;
  }else{
    return true;
  }
}

void MechDog::balancing_Task(void *p){
  MechDog *self = static_cast<MechDog*>(p);
  uint8_t first_flag = 1;
  mech_pose_t offset_angles;
  while(true){
    if(self->open_imu == 1){
      if(first_flag == 1){
        first_flag = 0;
        self->set_default_pose();
        vTaskDelay(1500);
      }
    }else{
      if(first_flag == 0){
        first_flag = 1;
        self->set_default_pose();
        vTaskDelay(1500);
      }
      vTaskDelete(NULL);
    }
    read_angle();
    if(abs(radianX_last) > 50 || abs(radianY_last) > 50){
      self->open_imu = 0;
      vTaskDelay(1000);
    }else{
      if(abs(radianX_last) > 1){
        offset_angles.orientation.roll = offset_angles.orientation.roll + radianX_last * 0.1 ;
        offset_angles.orientation.roll = offset_angles.orientation.roll < 24 ? offset_angles.orientation.roll : 24 ;
        offset_angles.orientation.roll = offset_angles.orientation.roll > -24 ? offset_angles.orientation.roll : -24;
      }
      if(abs(radianY_last) > 1){
        offset_angles.orientation.pitch = offset_angles.orientation.pitch - radianY_last * 0.1;
        offset_angles.orientation.pitch = offset_angles.orientation.pitch < 24 ? offset_angles.orientation.pitch : 24;
        offset_angles.orientation.pitch = offset_angles.orientation.pitch > -24 ? offset_angles.orientation.pitch : -24;
      }
      self->set_pose(offset_angles,20);
      vTaskDelay(20);
    }
  }
}


void read_angle(){
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax0 = ((float)(ax)) * 0.3 + ax0 * 0.7;  //对读取到的值进行滤波
  ay0 = ((float)(ay)) * 0.3 + ay0 * 0.7;
  az0 = ((float)(az)) * 0.3 + az0 * 0.7;
  ax1 = (ax0 - ax_offset) /  8192.0;  // 校正，并转为重力加速度的倍数
  ay1 = (ay0 - ay_offset) /  8192.0;
  az1 = (az0 - az_offset) /  8192.0;

  gx0 = ((float)(gx)) * 0.3 + gx0 * 0.7;  //对读取到的角速度的值进行滤波
  gy0 = ((float)(gy)) * 0.3 + gy0 * 0.7;
  gz0 = ((float)(gz)) * 0.3 + gz0 * 0.7;
  gx1 = (gx0 - gx_offset);  //校正角速度
  gy1 = (gy0 - gy_offset);
  gz1 = (gz0 - gz_offset);


  //互补计算x轴倾角
  radianX = atan2(ay1, az1);
  radianX_last = radianX * 180.0 / 3.1415926; //
  
  //互补计算y轴倾角
  radianY = atan2(ax1, az1);
  radianY_last = -radianY * 180.0 / 3.1415926; //
  
}