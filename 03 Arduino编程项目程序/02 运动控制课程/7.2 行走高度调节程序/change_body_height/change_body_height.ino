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
  mech_pose_t high = {
    {0,0,20},{0,0,0}
  };
  mech_pose_t low = {
    {0,0,-30},{0,0,0}
  };
  /* 
       transform()姿态变换函数
       参数1：平移身体（x,y,z轴）， 转动身体（绕x,y,z轴转动）
       参数2：变换的时间
  */
  switch (step) {
    case 0:
      mechdog.move(50,0);
      delay(5000);
      step++;
      break;
    case 1:
      mechdog.transform(high,1000);
      delay(5000);
      step++;
      break;
    case 2:
      mechdog.transform(low,1000);
      delay(5000);
      step++;
      break;
    case 3:
      mechdog.move(0,0);
      delay(500);
      mechdog.set_default_pose();
      step++;
      break;
  }
  delay(100);
}