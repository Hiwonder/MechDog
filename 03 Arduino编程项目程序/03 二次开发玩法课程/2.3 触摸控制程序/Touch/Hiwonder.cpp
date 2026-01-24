#include "esp32-hal-gpio.h"
#include "esp32-hal-adc.h"
#include "HardwareSerial.h"
#include "esp32-hal.h"
#include "Hiwonder.h"
#include <Wire.h>

TwoWire IIC1 = TwoWire(0);
TwoWire IIC2 = TwoWire(1);

bool wireWriteByte(TwoWire *iic, uint8_t addr, uint8_t val){ //写字节
  iic->beginTransmission(addr);
  iic->write(val);
    if(iic->endTransmission() != 0 ) 
    {
        return false;
    }
    return true;
}

bool wireWriteDataArray(TwoWire *iic, uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len){ //写多个字节
  unsigned int i;

  iic->beginTransmission(addr);
  iic->write(reg);
  for(i = 0; i < len; i++) 
  {
      iic->write(val[i]);
  }
  if( iic->endTransmission() != 0 ) 
  {
      return false;
  }
  return true;
}

int wireReadDataArray(TwoWire *iic, uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len){ //读指定长度字节
  unsigned char i = 0;  
  if (!wireWriteByte(iic,addr, reg)) 
  {
      return -1;
  }
  iic->requestFrom(addr, len);
  while (iic->available()) 
  {
      if (i >= len) 
      {
          return -1;
      }
      val[i] = iic->read();
      i++;
  }
  return i;
}

bool wireWriteWords(TwoWire *iic, uint8_t addr, uint8_t reg, uint8_t idNum, const char *words){ //写入ID及字符串
  iic->beginTransmission(ASR_I2C_ADDR);
  iic->write(ASR_ADD_WORDS_ADDR);
  iic->write(idNum);
  iic->write(reinterpret_cast<const uint8_t*>(words),strlen(words));
  if( iic->endTransmission() != 0 ) 
  {
    delay(10);
    return false;
  }
  delay(10);
  return true;
}

void taskRun(void *p){
  CallbackFunc func = (CallbackFunc)p;
  while(1){
    func();
    vTaskDelay(100);
  }
}

void startMain(CallbackFunc ncb){
  if(ncb == nullptr) return;
  xTaskCreate(             
    taskRun,          
    "userThread",       
    1024,           
    (void*)ncb,           
    1,              
    NULL
  );
}


/*PowerBuzzer*/
void PowerBuzzer::Buzzer_init(void){  //蜂鸣器初始化
  ledcSetup(Buzzer_channel,Buzzer_freq,10);
  ledcAttachPin(Buzzer_Pin, Buzzer_channel); 
  
  xTaskCreate(             //任务初始化
    Buzzer_Task,          // 任务函数
    "Task Buzzer",       // 任务名称
    1024,           // 任务堆栈大小
    this,           // 传递给任务函数的参数
    1,              // 任务优先级
    &Battery_TaskHandel    // 任务句柄
  );

}

void PowerBuzzer::disableLowPowerAlarm(void){  //关闭低压警报
  bt_open = false;
}

int PowerBuzzer::readBattery(void){  //读取电池电量
  BatteryValue = analogRead(34) * 3.6;
  if(bt_open){
    if(BatteryValue < 7000) bt_state = true;
  }
  return BatteryValue;
}

void PowerBuzzer::Buzzer_Task(void *p){  //电压监测+蜂鸣器任务
  PowerBuzzer *self = static_cast<PowerBuzzer*>(p);
  while(1){
    self->readBattery();
    if(self->bt_state){
      if(self->duty_flag){
        if(self->Buzzer_freq != 1000){
          self->Buzzer_freq = 1000;
          ledcSetup(self->Buzzer_channel,self->Buzzer_freq,10);
        }
        ledcWrite(self->Buzzer_channel, 500);
        self->duty_flag = false;
      }else {
        ledcWrite(self->Buzzer_channel, 0);
        self->duty_flag = true;
      }
      vTaskDelay(500);
    }else if (self->userTone) {
      ledcWrite(self->Buzzer_channel, self->duty_value);
      vTaskDelay(self->time_value);
      ledcWrite(self->Buzzer_channel, 0);
      self->userTone = false;
    }else{
      ledcWrite(self->Buzzer_channel, 0);
    }
    vTaskDelay(100);
  }
}

void PowerBuzzer::playTone(int duty, int btime, bool state){    //设置鸣响音调
  userTone = true;
  duty_value = duty;
  time_value = btime;
  if(state) delay(btime);
}
void PowerBuzzer::setVolume(int freq){    //设置音量[0,1000]
  Buzzer_freq = freq;
  ledcSetup(Buzzer_channel,Buzzer_freq,10);
}

/*Button*/
void Button::Button_init(uint8_t num){
  if(num == 1){
    pinMode(Key_Pin, INPUT_PULLUP);
    bt_key = Key_Pin;
  }else{
    pinMode(Touch_Pin, INPUT_PULLUP);
    bt_key = Touch_Pin;
  }

  xTaskCreate(             
    Button_Task,          
    "Task Button",       
    1024,           
    this,           
    1,              
    &Button_TaskHandel    
  );
}

void Button::Button_Task(void *p){
  Button *self = static_cast<Button*>(p);
  int bt_count = 0;
  int long_press = 0;
  while(1){
    if(!digitalRead(self->bt_key)){
      bt_count++;
      if(bt_count > 2) self->clicked_state = true;
      if(bt_count >= 20){
        if(long_press == 0){
          long_press = 1;
          if(self->longpressed_cb != NULL) self->longpressed_cb();
        }
      }
    }else{
      self->clicked_state = false;
      if(bt_count >= 3 && bt_count < 20){
        if(self->clicked_cb != NULL) self->clicked_cb();
      }
      long_press = 0;
      bt_count = 0;
    }
    vTaskDelay(20);
  }
}

int Button::GetButtonResult(void){
  return clicked_state;
}

void Button::Clicked(CallbackFunc ncb){
  if(ncb != nullptr) clicked_cb = ncb;
}

void Button::Longpressed(CallbackFunc ncb){
  if(ncb != nullptr) longpressed_cb = ncb;
}

/*LED*/
void LED::LED_init(void){
  pinMode(Led_Pin,OUTPUT);
}
void LED::on(void){
  digitalWrite(Led_Pin,LOW);
}
void LED::off(void){
  digitalWrite(Led_Pin,HIGH);
}

/*LightSensor*/
int LightSensor::read(void){
  return map(analogRead(33), 0, 4096, 255, 0);
}

/* UltrasoundSonar */
void UltrasoundSonar::Ultrasound_init(void){
  IIC1.begin(SDA1, SCL1);
}

/* 
  设置灯的颜色
	参数1：0表示两边，1表示左边，2表示右边
	参数2：颜色的rgb比例值，以元组形式传入,范围0-255, 依次为r， g， b
*/
void UltrasoundSonar::setRGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b){
  uint8_t RGB[3]; 
  uint8_t value = RGB_WORK_SIMPLE_MODE; //普通模式
  wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB_WORK_MODE,&value,1);
  if(index == 0){
    RGB[0] = r;RGB[1] = g;RGB[2] = b;//RGB1
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB1_R,RGB,3);
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB2_R,RGB,3); 
  }else{
    RGB[0] = r;RGB[1] = g;RGB[2] = b;//RGB1
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, (index == 1 ? RGB1_R : RGB2_R),RGB,3);
  }
}

/* 
  呼吸灯模式
  参数1：0表示两边的灯，1表示左边,2表示右边
  参数2：颜色通道， 0表示r，1表示g， 2表示b
  参数3：颜色变化周期，单位100ms，例如设置3000ms周期，该参数设置为30
*/
void UltrasoundSonar::setBreathing(uint8_t index, uint8_t rgb, uint8_t cycle){ 
  uint8_t value = RGB_WORK_BREATHING_MODE; //呼吸灯模式 
  wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB_WORK_MODE, &value, 1);
  if(index == 0){
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB1_R_BREATHING_CYCLE + rgb, &cycle,1); 
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, RGB2_R_BREATHING_CYCLE + rgb, &cycle,1); 
  }else{
    wireWriteDataArray(&IIC1, ULTRASOUND_I2C_ADDR, (index == 1 ? RGB1_R_BREATHING_CYCLE : RGB2_R_BREATHING_CYCLE) + rgb, &cycle,1); 
  }
}

uint16_t UltrasoundSonar::getDistance(){ //获取超声波测得的距离，单位mm
  uint16_t distance;
  int filter_sum = 0;
  wireReadDataArray(&IIC1, ULTRASOUND_I2C_ADDR, DISTANCE_ADDR,(uint8_t *)&distance,2);
  if(distance == DISTANCE_ERRO) distance = 5000;
  filter_buf[FILTER_N] = distance;     //读取超声波测值
  
  for(int i = 0; i < FILTER_N; i++) {    
    filter_buf[i] = filter_buf[i + 1];               // 所有数据左移，低位仍掉
    filter_sum += filter_buf[i];
  }
  return (uint16_t)(filter_sum / FILTER_N) / 10;
}

/* ASRSensor */
void ASRSensor::ASR_init(){
  IIC2.begin(SDA2, SCL2);
}

void ASRSensor::setMode(uint8_t mode){
  wireWriteDataArray(&IIC2, ASR_I2C_ADDR, ASR_MODE_ADDR, &mode, 1);
}

void ASRSensor::addWord(uint8_t idNum, const char *words){
  wireWriteWords(&IIC2, ASR_I2C_ADDR, ASR_ADD_WORDS_ADDR, idNum, words);
}

void ASRSensor::erase(){
  wireWriteDataArray(&IIC2, ASR_I2C_ADDR, ASR_WORDS_ERASE_ADDR, NULL, 0);
  delay(60);
}

uint8_t ASRSensor::getResult(){
  uint8_t result;
  wireReadDataArray(&IIC2, ASR_I2C_ADDR, ASR_RESULT_ADDR, &result, 1);
  return result;
}

/* MP3Sensor */
void MP3Sensor::MP3_init(void){
  IIC1.begin(SDA1, SCL1);
}

void MP3Sensor::play(uint16_t num){
  if(num == MP3_MAXCOUNT){
    wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_PLAY_ADDR, NULL, 0);
  }else{
    wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_PLAY_NUM_ADDR, (uint8_t*)&num, 2);
  }
  delay(20);
}


void MP3Sensor::pause(void){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_PAUSE_ADDR, NULL, 0);
  delay(20);
}

void MP3Sensor::prev(void){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_PREV_ADDR, NULL, 0);
  delay(20);
}

void MP3Sensor::next(void){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_NEXT_ADDR, NULL, 0);
  delay(20);
}

void MP3Sensor::loop_on(void){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_SINGLE_LOOP_ON_ADDR, NULL, 0);
  delay(20);
}

void MP3Sensor::loop_off(void){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_SINGLE_LOOP_OFF_ADDR, NULL, 0);
  delay(20);
}

void MP3Sensor::volume(uint8_t value){
  wireWriteDataArray(&IIC1, MP3_I2C_ADDR, MP3_VOL_VALUE_ADDR, &value, 1);
  delay(20);
}

/* ESP32S3Cam */
void ESP32S3Cam::ESP32S3_init(){
  IIC2.begin(SDA2,SCL2);
}

bool ESP32S3Cam::face_recognition(){
  uint8_t center[2];
  wireReadDataArray(&IIC2, ESP32S3_IIC_ADDR, FACE_CENTER_ADDR, center, 2);
  if (center[0] != 0 && center[1] != 0) return true;
  else return false;
}

void ESP32S3Cam::color_recognition(uint8_t *color_data){
  uint8_t color[7];
  wireReadDataArray(&IIC2, ESP32S3_IIC_ADDR, COLOR_ADDR, color, 7);
  for(int i= 0; i < 5; i++){
    if(color[i] == i){
      color_data[i] = i;
    }else{
      color_data[i] = 255;
    }
  }
}

void ESP32S3Cam::color_follow(uint8_t color_label, uint8_t *color){
  if(color_label > 5){
    return;
  }
  uint8_t color_result[7];
  wireReadDataArray(&IIC2, ESP32S3_IIC_ADDR, (COLOR_FOLLOW_ADDR + color_label), color_result, 7);
  for(int i = 0; i < 7; i++){
    color[i] = color_result[i];
  }
}

void ESP32S3Cam::line_follow(uint8_t line_num, uint8_t *line){
  uint8_t line_up_result[7];
  uint8_t line_down_result[7];
  if(line_num <= 5){
    wireReadDataArray(&IIC2, ESP32S3_IIC_ADDR, (Line_FOLLOW_UP_ADDR + (line_num * 2)), line_up_result, 7);
    wireReadDataArray(&IIC2, ESP32S3_IIC_ADDR, (Line_FOLLOW_DOWN_ADDR + (line_num * 2)), line_down_result, 7);
    delay(10);
    for(int i = 0; i < 14; i++){
      if(i<7){
        line[i] = line_up_result[i];
      }else{
        line[i] = line_down_result[i-7];
      }
    }
  }
}
