#include "mech_base_types.h"
#include "HW_MechDog.h"

int8_t step = 0;

MechDog mechdog;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  mech_pose_t roll[2] = {
    {{0,0,0},{15,0,0}},
    {{0,0,0},{-30,0,0}}
  };
  mech_pose_t pitch[2] = {
    {{0,0,0},{0,15,0}},
    {{0,0,0},{0,-30,0}}
  };
  /* 
       transform()姿态变换函数
       参数1：平移身体（x,y,z轴），转动身体（绕x,y,z轴转动）
       参数2：变换的时间
  */
  switch (step) {
    case 0:
      mechdog.transform(roll[0],500); //绕X轴旋转
      delay(2000);
      mechdog.transform(roll[1],1000);
      delay(2000);
      mechdog.set_default_pose();
      delay(2000);
      step++;
      break;
    case 1:
      mechdog.transform(pitch[0],500); //绕Y轴旋转
      delay(2000);
      mechdog.transform(pitch[1],1000);
      delay(2000);
      mechdog.set_default_pose();
      delay(2000);
      step++;
      break;
  }
  delay(100);
}