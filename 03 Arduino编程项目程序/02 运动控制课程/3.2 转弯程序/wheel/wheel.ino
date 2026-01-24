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
     move()函数
     参数1：步幅（单位mm）（正值为向前，负值为向后）；
     参数2：转弯角度（单位：度），正值为左转，负值为右转
  */
  switch (step) {
    case 0:
      mechdog.move(50,20); //左转
      delay(5000);
      mechdog.move(0,0);
      delay(2000);
      mechdog.move(50,-20); //右转
      delay(5000);
      mechdog.move(0,0); //停止
      delay(2000);
      step++;
      break;
  }
  delay(100);
}