#include "mech_base_types.h"
#include "HW_MechDog.h"

//偏差读取数组，用于存储读取到的偏差值。
int8_t result_offset[9];

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
  mechdog.read_all_offset(result_offset); //读取MechDog保存的偏差，并存储到result_offset内。
  Serial.print("{");
  for (int i = 0; i<9; i++) {
    Serial.print(result_offset[i]);
    if(i<8) Serial.print(",");
  }
  Serial.println("}");
  delay(1000);
}
