#include "mech_base_types.h"
#include "HW_MechDog.h"

/* 
   由于下载Arduino程序时会对ESP32的MicroPython固件进行擦除，从而导致原先的舵机偏差被清除。
   所以在下载程序前，需要先前往上位机查看当前的舵机偏差，并将舵机偏差记录下来，再填入偏差数组内。
   偏差设置程序只需要下载到MechDog内运行一次，即可存储在MechDog的Arduino编程环境中。
*/

//偏差数组：第0位是转向偏差（大于0表示向左偏，小于0表示向右偏），第1-8位是PWM舵机1-8号的偏差。
int8_t MechDog_offset[9] = {-3, -11, -24, -31, -2, -21, 30, -23, 15};
//偏差读取数组，用于存储读取到的偏差值。
int8_t result_offset[9];
int8_t step = 0;

MechDog mechdog;

void setup() {
  Serial.begin(115200);
  mechdog.MechDog_init();  //对MechDog进行初始化
  delay(1000);
}

void loop() {
  userTask();
}

void userTask(){
  switch(step){
    case 0:
      mechdog.setsave_all_offset(MechDog_offset); //设置MechDog的偏差并进行保存。
      delay(500);
      step++;
      break;
    case 1:
      mechdog.read_all_offset(result_offset); //读取MechDog保存的偏差，并存储到result_offset内。
      Serial.print("{");
      for (int i = 0; i<9; i++) {
        Serial.print(result_offset[i]);
        if(i<8) Serial.print(",");
      }
      Serial.println("}");
      break;
  }
  delay(1000);
}
