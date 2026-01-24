#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建语音识别模块对象
ASRSensor myasr;
//创建MP3模块对象
MP3Sensor mp3;

uint8_t recognize_result = 0;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  myasr.ASR_init(); //初始化语音识别模块
  mp3.MP3_init(); //初始化MP3模块对象
  delay(1000);

  //设置为循环识别模式
  myasr.setMode(1);
  //设置MP3音量为30
  mp3.volume(30);

  if(true){ //设置语音识别词条，只需要设置一次，后续的可以改为False
    myasr.addWord(0,"ni hao");
    myasr.addWord(1,"xiang qian");
    myasr.addWord(2,"xiang hou");
    myasr.addWord(3,"xiang zuo");
    myasr.addWord(4,"xiang you");
    myasr.addWord(5,"ting zhi");
  }
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  recognize_result = myasr.getResult();
  //识别到向前
  if(recognize_result == 1){
    Serial.println(recognize_result);
    mp3.play(1);
    mp3.play();
    mechdog.move(80,0);
    delay(3000);
    mechdog.move(0,0);
    delay(2000);
  }
  //识别到向后
  if(recognize_result == 2){
    Serial.println(recognize_result);
    mp3.play(2);
    mp3.play();
    mechdog.move(-80,0);
    delay(3000);
    mechdog.move(0,0);
    delay(2000);
  }
  //识别到向左
  if(recognize_result == 3){
    Serial.println(recognize_result);
    mp3.play(3);
    mp3.play();
    mechdog.move(80,30);
    delay(3000);
    mechdog.move(0,0);
    delay(2000);
  }
  //识别到向右
  if(recognize_result == 4){
    Serial.println(recognize_result);
    mp3.play(1);
    mp3.play();
    mechdog.move(80,-30);
    delay(3000);
    mechdog.move(0,0);
    delay(2000);
  }
  delay(50);
}
