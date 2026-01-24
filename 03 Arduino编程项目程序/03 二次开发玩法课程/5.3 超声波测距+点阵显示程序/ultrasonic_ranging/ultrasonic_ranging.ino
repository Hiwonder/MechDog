#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建发光超声波传感器对象
UltrasoundSonar ult;
//创建LED点阵模块对象
WMMatrixLed tm(32,33);
//超声波测量距离
uint16_t distance = 0;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波
  
  tm.setBrightness(4); //设置亮度
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  //获取发光超声波测量的距离
  distance = ult.getDistance();
  //点阵模块显示距离
  tm.showNum((float)distance,0); 
  //Serial.println(distance);
  //若距离小于15mm
  if(distance <= 15){
    //发光超声波设置颜色函数
    //参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
    //参数2、3、4：对应红、绿、蓝3种颜色值
    ult.setRGB(0,0xff,0x00,0x00); //设置为红色
   }
   else{
    if(distance > 30){
      ult.setRGB(0,0x00,0x00,0x99); //设置为蓝色
    }else{
      ult.setRGB(0,0xfd,0xd0,0x00); //设置为黄色
    }
  }
  delay(100);
}
