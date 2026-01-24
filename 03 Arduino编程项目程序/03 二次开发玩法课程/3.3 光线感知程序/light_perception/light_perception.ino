#include "mech_base_types.h"
#include "HW_MechDog.h"

MechDog mechdog;
//创建亮度传感器对象
LightSensor light;

//光照亮度阈值
uint16_t Intensity_threshold = 100;
//读取的亮度值
uint16_t brightness = 0;

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
  //读取光照强度
  brightness = light.read();
  //若亮度值大于阈值
  if(brightness >= Intensity_threshold){
    //执行站立动作
    //mechdog.action_run("stand_four_legs");
    mechdog.set_default_pose();
    delay(2000);
    //行走
    mechdog.move(80,0);
    delay(1000);
    //当亮度值大于阈值，则一直等待，直到小于时则跳出循环。
    while(light.read() > Intensity_threshold){
      delay(100);
    }
  }else{
    //停下
    mechdog.move(0,0);
    delay(2000);
    //执行趴下动作组
    mechdog.action_run("go_prone");
    delay(1000);
    //当亮度值小于阈值，则一直等待，直到大于时则跳出循环。
    while(light.read() < Intensity_threshold){
      delay(100);
    }
  }
}