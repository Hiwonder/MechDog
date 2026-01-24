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
  /* 
      set_gait_params()函数
      设置步态参数，参数内容如下：
      参数1：脚尖离地的时间；
      参数2：脚尖接触地面的时间；
      参数3：抬腿的高度。
  */
  switch (step) {
    case 0:
      mechdog.set_gait_params(150,350,20);
      mechdog.move(50,0);
      delay(5000);
      step++;
      break;
    case 1:
      mechdog.set_gait_params(200,600,50);
      mechdog.move(50,0);
      delay(5000);
      step++;
      break;
  }
  mechdog.move(0,0);
  delay(3000);
}