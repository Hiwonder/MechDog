#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建发光超声波传感器对象
UltrasoundSonar ult;

//超声波测量距离
uint16_t distance = 0;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波模块
  delay(1000);
  startMain(BuzzerTask); 
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  //获取发光超声波测量的距离
  distance = ult.getDistance();
  //Serial.println(distance);
  //若距离小于10mm
  if(distance <= 10){
    //发光超声波设置颜色函数
    //参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
    //参数2、3、4：对应红、绿、蓝3种颜色值
    ult.setRGB(0,255,0,0); //设置为红色
   }
   else{
    if(distance > 50){
      ult.setRGB(0,0,255,0); //设置为绿色
    }else{
      ult.setRGB(0,(250-((round(distance))*5)),((round(distance))*5),0); 
    }
  }
  delay(100);
}

void BuzzerTask(){
  if(distance <= 50){
    mechdog.playTone(800,100,true); 
    delay(distance*20);
  }
}