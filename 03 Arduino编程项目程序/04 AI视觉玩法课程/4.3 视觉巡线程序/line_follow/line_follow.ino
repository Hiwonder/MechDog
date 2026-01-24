#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建发光超声波模块对象
UltrasoundSonar ult;
//创建ESP32S3视觉模块对象
ESP32S3Cam cam;

uint8_t line_data[14]; //用于存储巡线数据
uint8_t centerX;

uint8_t last_centerX; //用于存储最后一次识别到的坐标

mech_pose_t pose = {
  {-5,0,0},{0,0,0}
};


void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波模块
  cam.ESP32S3_init(); //初始化ESP32S3视觉模块
  delay(1000);
  ult.setRGB(0,0x00,0x00,0x00); //关闭发光超声波的RGB彩灯
  mechdog.set_gait_params(150,450,40); //设置步态
  mechdog.transform(pose,80); //将重心后移
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  /*
    line_follow()函数
    参数1：设置巡线的颜色
    参数2：存储线路数据的数组，该数组的长度最少为14
  */
  cam.line_follow(YELLOW,line_data);
  if(line_data[0] == YELLOW){ //若识别到设置的颜色
    centerX = line_data[1]; //获取线路中心点
    if(centerX < 70){ //左转
      mechdog.move(80,35);
      last_centerX = centerX; //保存最后一次识别到的坐标
    }else if(centerX > 110){ //右转
      mechdog.move(80, -35);
      last_centerX = centerX; //保存最后一次识别到的坐标
    }
  }else if(last_centerX < 100 && last_centerX != 0){ //当识别不到线路时，进行丢线矫正
    mechdog.move(80, 35);
    last_centerX = 0;
    delay(500);
  }else if(last_centerX > 100){ 
    mechdog.move(80, -35);
    last_centerX = 0;
    delay(500);
  }else{
    mechdog.move(80,0);
  }
  delay(10);
}
