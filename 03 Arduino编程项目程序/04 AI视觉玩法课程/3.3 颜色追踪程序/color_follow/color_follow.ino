#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建ESP32S3视觉模块对象
ESP32S3Cam cam;

int8_t angle = 0;
int8_t dir = 1;
int16_t wide;
int16_t high;
int16_t area;

uint8_t color_data[7]; //用于存储视觉模块反馈的颜色数据

mech_pose_t pose = {
  {-5,0,0},{0,0,0}
};

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  cam.ESP32S3_init(); //初始化ESP32S3视觉模块
  delay(1000);
  mechdog.set_gait_params(150,450,40); //设置步态
  mechdog.transform(pose,80); //将重心后移
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  // RED \ YELLOW \ GREEN \ BLUE \ BLACK
  /*
    参数1：用于设置识别的颜色
    参数2：接收识别到的数据
  */
  cam.color_follow(GREEN,color_data); //读取颜色数据
  if(color_data[0] == GREEN){
    if(color_data[5] < 60){
      angle = 25;
    }else if(color_data[5] > 100){
      angle = -25;
    }

    if(color_data[6] < 70){
      dir = 1;
    }else{
      dir = -1;
    }
    
    //计算颜色的面积
    wide = color_data[3] - color_data[1];
    high = color_data[4] - color_data[2];
    area = wide * high;

    if(area > 5000){
      mechdog.move(0,0);
    }else{
      if(dir == 1){
        mechdog.move(50,angle);
      }else{
        mechdog.move(-50,0);
      }
    }

  }else{
    mechdog.move(0,0);
  }
  delay(100);
}
