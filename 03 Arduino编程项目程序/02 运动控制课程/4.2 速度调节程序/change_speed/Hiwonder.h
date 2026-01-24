#include "Wire.h"
#ifndef HIWONDER_H
#define HIWONDER_H

#include <Arduino.h>

#define Freq_default 1000
#define Channel_default 11
#define Pin_default 21
#define Key_Pin 5
#define Touch_Pin 33
#define Led_Pin 18
#define SCL1 23
#define SDA1 22
#define SCL2 13
#define SDA2 19

/* Ultrasound */
#define ULTRASOUND_I2C_ADDR 0x77  //发光超声波模块IIC地址
#define DISTANCE_ADDR 0
#define DISDENCE_L    0
#define DISDENCE_H    1
#define RGB_WORK_MODE 2//RGB灯模式，0：用户自定义模式   1：呼吸灯模式  默认0
#define RGB1_R      3//1号探头的R值，0~255，默认0
#define RGB1_G      4//默认0
#define RGB1_B      5//默认255
#define RGB2_R      6//2号探头的R值，0~255，默认0
#define RGB2_G      7//默认0
#define RGB2_B      8//默认255
#define RGB1_R_BREATHING_CYCLE      9 //呼吸灯模式时，1号探头的R的呼吸周期，单位100ms 默认0，
#define RGB1_G_BREATHING_CYCLE      10
#define RGB1_B_BREATHING_CYCLE      11
#define RGB2_R_BREATHING_CYCLE      12//2号探头
#define RGB2_G_BREATHING_CYCLE      13
#define RGB2_B_BREATHING_CYCLE      14
#define RGB_WORK_SIMPLE_MODE    0 //灯光设置模式
#define RGB_WORK_BREATHING_MODE   1 //呼吸灯模式
#define FILTER_N 3
#define DISTANCE_ERRO 65535

/* ASRSensor */
#define ASR_I2C_ADDR		0x79 //语音识别模块IIC地址
#define ASR_RESULT_ADDR           100
//识别结果存放处，通过不断读取此地址的值判断是否识别到语音，不同的值对应不同的语音，
#define ASR_WORDS_ERASE_ADDR      101//擦除所有词条
#define ASR_MODE_ADDR             102
//识别模式设置，值范围1~3
//1：循环识别模式。状态灯常亮（默认模式）    
//2：口令模式，以第一个词条为口令。状态灯常灭，当识别到口令词时常亮，等待识别到新的语音,并且读取识别结果后即灭掉
//3：按键模式，按下开始识别，不按不识别。支持掉电保存。状态灯随按键按下而亮起，不按不亮
#define ASR_ADD_WORDS_ADDR        160//词条添加的地址，支持掉电保存

/* MP3Sensor */
#define MP3_I2C_ADDR    0x7B //MP3模块IIC地址
#define MP3_PLAY_NUM_ADDR         1//指定曲目播放，0~3000，低位在前，高位在后
#define MP3_PLAY_ADDR             5//播放
#define MP3_PAUSE_ADDR            6//暂停
#define MP3_PREV_ADDR             8//上一曲
#define MP3_NEXT_ADDR             9//下一曲
#define MP3_VOL_VALUE_ADDR        12//指定音量大小0~30
#define MP3_SINGLE_LOOP_ON_ADDR   13//开启单曲循环,要在播放过程开启才有效
#define MP3_SINGLE_LOOP_OFF_ADDR  14//关闭单曲循环
#define MP3_MAXCOUNT  3001

#define ESP32S3_IIC_ADDR 0x53
#define RED 0
#define YELLOW 1
#define GREEN 2
#define BLUE 3
#define BLACK 4
#define FACE_CENTER_ADDR 0xB0
#define COLOR_ADDR 0xC5
#define COLOR_FOLLOW_ADDR 0xC0
#define Line_FOLLOW_UP_ADDR 0xA0
#define Line_FOLLOW_DOWN_ADDR 0xA1


static int filter_buf[FILTER_N + 1];
typedef void (*CallbackFunc)();

bool wireWriteByte(TwoWire *iic, uint8_t addr, uint8_t val);
bool wireWriteDataArray(TwoWire *iic, uint8_t addr, uint8_t reg,uint8_t *val,unsigned int len);
int wireReadDataArray(TwoWire *iic, uint8_t addr, uint8_t reg, uint8_t *val, unsigned int len);
bool wireWriteWords(TwoWire *iic, uint8_t addr, uint8_t reg,uint8_t idNum,unsigned char *words);

void taskRun(void *p);
void startMain(CallbackFunc ncb);

extern TwoWire IIC1;
extern TwoWire IIC2;

class PowerBuzzer{
  private:
    int Buzzer_freq;
    int Buzzer_channel;
    int Buzzer_Pin;
    int BatteryValue;
    int duty_value;
    int time_value;
    bool bt_open;
    bool bt_state;
    bool duty_flag;
    bool userTone;
    TaskHandle_t Battery_TaskHandel;
    static void Buzzer_Task(void *p);
  
  protected:
    void Buzzer_init(void);

  public:
    PowerBuzzer(void){
      Buzzer_freq = Freq_default; 
      Buzzer_channel = Channel_default; 
      Buzzer_Pin = Pin_default;
      bt_open = true;
      bt_state = false;
      duty_flag = false;
      userTone = false;
      Battery_TaskHandel = NULL;
    };
    
    void playTone(int duty, int btime, bool bg_State);
    void setVolume(int freq);
    int readBattery(void);
    void disableLowPowerAlarm(void);
};

class Button{
  private:
    int bt_key;
    bool clicked_state;
    CallbackFunc clicked_cb;
    CallbackFunc longpressed_cb;
    TaskHandle_t Button_TaskHandel;
    static void Button_Task(void *p);
  public:
    Button(void){
      bt_key = 0;
      clicked_state = false;
      clicked_cb = nullptr;
      longpressed_cb = nullptr;
      Button_TaskHandel = NULL;
    };
    int GetButtonResult(void);
    void Button_init(uint8_t num = 1);
    void Clicked(CallbackFunc ncb);
    void Longpressed(CallbackFunc ncb);
};

class LED{
  public:
    void LED_init(void);
    void on(void);
    void off(void);
};

class LightSensor{
  public:
    int read(void);
};

class UltrasoundSonar{
  public:
    void Ultrasound_init(void);
    void setRGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
    void setBreathing(uint8_t index, uint8_t rgb, uint8_t cycle);
    uint16_t getDistance(void);
};

class ASRSensor{
  public:
    void ASR_init(void);
    void setMode(uint8_t mode);
    void addWord(uint8_t idNum, const char *words);
    void erase(void);
    uint8_t getResult(void);
};

class MP3Sensor{
  public:
    void MP3_init(void);
    void play(uint16_t num = MP3_MAXCOUNT);
    void pause(void);
    void prev(void);
    void next(void);
    void loop_on(void);
    void loop_off(void);
    void volume(uint8_t value);
};

class ESP32S3Cam{
  public:
    void ESP32S3_init();
    bool face_recognition();
    void color_recognition(uint8_t *color_data);
    void color_follow(uint8_t color_label, uint8_t *color);
    void line_follow(uint8_t line_num, uint8_t *line);
};

#endif  //HIWONDER_H
