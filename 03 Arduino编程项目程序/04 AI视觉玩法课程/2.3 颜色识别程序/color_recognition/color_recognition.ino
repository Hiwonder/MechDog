#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建发光超声波对象
UltrasoundSonar ult;
//创建ESP32S3视觉模块对象
ESP32S3Cam cam;

uint8_t color[5]; //用于存储视觉模块反馈的颜色数据

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波模块
  cam.ESP32S3_init(); //初始化ESP32S3视觉模块
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  cam.color_recognition(color);
  for(int i = 0; i < 5; i++){
    if(color[i] == RED){  //识别到红色
      ult.setRGB(0,255,0,0);
    }else if(color[i] == GREEN){ //识别到绿色
      ult.setRGB(0,0,255,0);
    }else if(color[i] == BLUE){ //识别到蓝色
      ult.setRGB(0,0,0,255);
    }
  }
  delay(100);
}
