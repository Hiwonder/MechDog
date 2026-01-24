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
     action_run()函数
     参数：需要运行的动作组名称，可前往本节内容文档下查看
  */
  switch (step) {
    case 0:
      mechdog.action_run("left_foot_kick"); //执行左脚踢球动作组
      delay(3000);
      step++;
      break;
  }
  delay(100);
}