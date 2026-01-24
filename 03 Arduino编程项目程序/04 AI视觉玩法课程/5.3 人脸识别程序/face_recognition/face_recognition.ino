#include "mech_base_types.h"
#include "HW_MechDog.h"

//创建MechDog对象
MechDog mechdog;
//创建ESP32S3视觉模块对象
ESP32S3Cam cam;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  cam.ESP32S3_init(); //初始化ESP32S3视觉模块
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  // RED \ YELLOW \ GREEN \ BLUE \ BLACK
  /*
    face_recognition()函数：
    若识别到人脸返回true，否则返回false
  */
  if(cam.face_recognition()){
    mechdog.action_run("scrape_a_bow");
    delay(5000);
  }
  delay(100);
}
