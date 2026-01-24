#include "mech_base_types.h"
#include "HW_MechDog.h"

int8_t step = 0;

int8_t enter_flag = 0;
int8_t speed = 40;

MechDog mechdog;
Button btn;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init(); //初始化MechDog
  
  btn.Button_init(1); //初始化按键，参数1表示为按键功能
  btn.Clicked(on_button1_clicked);
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  /* 
     move()函数
     参数1：步幅（单位mm）（正值为向前，负值为向后）；
     参数2：转弯角度（单位：度），正值为左转，负值为右转
  */
  if(enter_flag == 1){
    switch (step) {
      case 0:
        mechdog.move(speed,0);
        delay(5000);
        speed += 20;
        step++;
        break;
      case 1:
        mechdog.move(speed,0);
        delay(5000);
        speed += 20;
        step++;
        break;
      case 2:
        mechdog.move(speed,0);
        delay(5000);
        speed += 20;
        step++;
        break;
      case 3:
        mechdog.move(speed,0);
        delay(5000);
        speed = 40;
        step = 0;
        break;
    }
    mechdog.move(0,0);
    enter_flag = 0;
  }
  delay(100);
}

/* 按键回调函数 */
void on_button1_clicked(){
  enter_flag = 1;
}