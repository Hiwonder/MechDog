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
  
  btn.Button_init(2); //初始化按键，参数2表示为触摸传感器功能
  btn.Clicked(on_button1_clicked);
  delay(1000);
}

void loop() {
  userTask();
}

/* 用户函数 */
void userTask(){
  if(enter_flag == 1){
    switch (step) {
      case 0:
        //执行默认动作组：坐下
        mechdog.action_run("sit_dowm");
        delay(1500);
        step++;
        break;
      case 1:
        //执行默认动作组：趴下
        mechdog.action_run("go_prone");
        delay(1500);
        step++;
        break;
      case 2:
        //执行默认动作组：站立
        mechdog.action_run("stand_four_legs");
        delay(1500);
        step = 0;
        break;
    }
    enter_flag = 0;
  }
  delay(100);
}

/* 按键回调函数 */
void on_button1_clicked(){
  enter_flag = 1;
}