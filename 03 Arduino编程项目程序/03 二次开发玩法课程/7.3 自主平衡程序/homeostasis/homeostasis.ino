#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建发光超声波对象
UltrasoundSonar ult;

uint8_t step = 0;

uint8_t recognize_result = 0;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波模块
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  switch (step) {
    case 0:
      // 发光超声波设置颜色函数
      // 参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
      // 参数2、3、4：对应红、绿、蓝3种颜色值
      ult.setRGB(0,0xff,0xcc,0x33);
      // 开启自平衡状态
      mechdog.homeostasis(true);
      delay(2000);
      // 检测是否还在自平衡状态，当退出自平衡状态时即退出该循环
      while (mechdog.read_homeostasis_status()){
        delay(100);
      }
      ult.setRGB(0,0x33,0x33,0xff);
      mechdog.playTone(800, 100, true);
      step++;
    }
}
