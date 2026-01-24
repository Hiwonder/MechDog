#include "mech_base_types.h"
#include "HW_MechDog.h"
#define rxPin 32
#define txPin 33

typedef struct Object{
  MechDog* mechdog;
  UltrasoundSonar* ult;
};
extern MPU6050 accelgyro;

MechDog mechdog; //创建MechDog对象
UltrasoundSonar ult; //创建发光超声波对象

Object obj;

HardwareSerial mySerial(1); //创建串口实例

mech_pose_t attitude[8] = {
  {{10,0,0},{0,0,0}}, //重心前移姿态
  {{-10,0,0},{0,0,0}}, //重心后移姿态
  {{0,0,0},{1,0,0}}, //Roll +
  {{0,0,0},{-1,0,0}}, //Roll -
  {{0,0,0},{0,1,0}}, //Pitch +
  {{0,0,0},{0,-1,0}}, //Pitch -
  {{0,0,1},{0,0,0}}, //High +
  {{0,0,-1},{0,0,0}}, //High -
};



static void run_task(void *p); //动作运行任务

int8_t step = 0;
int8_t avo_flg = -1; //避障标志位
int8_t forward_flag = 1; //避障姿态切换标志位
int8_t action_num = -1; //运动
int8_t dir_flag = 1; //运动姿态切换标志位
int8_t actions = 0; //动作组
int8_t actions_flg = 0; //动作组运行标志位
int8_t attitude_flg = 0; //姿态调节标志位
int8_t homeostasis_flg = 0; //自平衡功能

//姿态调节角度
int8_t Pitch_angle = 0;
int8_t Roll_angle = 0;
int8_t High_mm = 0;

int8_t rec_data[3];

void setup() {
  mySerial.begin(9600,SERIAL_8N1,rxPin,txPin); //初始化串口
  obj.mechdog = &mechdog;
  obj.ult = &ult;
  Project_init(); //初始化MechDog和发光超声波模块
  delay(100);
}

void loop() {
  uint8_t index = 0;
  while (mySerial.available() > 0) {
    String cmd = mySerial.readString(); //读取串口数据
    if(cmd.startsWith("CMD") && cmd.endsWith("$")){ //进行数据校验
      cmd = cmd.substring(cmd.indexOf('|') + 1,cmd.indexOf('$')); 
      while(cmd.indexOf("|") != -1){
        rec_data[index] = cmd.substring(0, cmd.indexOf('|')).toInt(); //提取数据字符串并转换为int类型
        cmd = cmd.substring(cmd.indexOf('|') + 1);
        index++;
      }
      switch(rec_data[0]){
        case 1: //姿态调节
          if(index == 2 && rec_data[1] == 5){
            mechdog.set_default_pose();
            vTaskDelay(1000);
          }else if(index == 3){
            switch(rec_data[1]){
              case 1: //Pitch调节
                if(abs(rec_data[2]) == 1){
                  if(rec_data[2] == 1 && Pitch_angle < 17){
                    Pitch_angle++;
                    mechdog.transform(attitude[4],80);
                  }else if(rec_data[2] == -1 && Pitch_angle > -17){
                    Pitch_angle--;
                    mechdog.transform(attitude[5],80);
                  }
                }
                break;
              case 2: //Roll调节
                if(abs(rec_data[2]) == 1){
                  if(rec_data[2] == 1 && Roll_angle < 17){
                    Roll_angle++;
                    mechdog.transform(attitude[2],80);
                  }else if(rec_data[2] == -1 && Roll_angle > -17){
                    Roll_angle--;
                    mechdog.transform(attitude[3],80);
                  }
                }
                break;
              case 3: //自平衡
                if(rec_data[2] == 1){
                  homeostasis_flg = 1;
                }else{
                  homeostasis_flg = 0;
                }
                break;
              case 4: //Roll调节
                if(abs(rec_data[2]) == 1){
                  if(rec_data[2] == 1 && High_mm < 15){
                    High_mm++;
                    mechdog.transform(attitude[6],80);
                  }else if(rec_data[2] == -1 && High_mm > -25){
                    High_mm--;
                    mechdog.transform(attitude[7],80);
                  }
                }
                break;
            }
          }
          break;

        case 2: //动作组调用
          if(index == 3){
            if(rec_data[1] == 1){
              actions_flg = 1;
              actions = rec_data[2];
            }else if(rec_data[1] == 2){
              actions_flg = 2;
              actions = rec_data[2];
            }
          }
          break;

        case 3: //运动控制
          if(index == 2){
            action_num = rec_data[1];
            if(action_num < 6 && dir_flag != 1){
              dir_flag = 1;
              mechdog.transform(attitude[0],100);
            }else{
              dir_flag = -1;
              mechdog.transform(attitude[1],100);
            }
          }
          break;

        case 4: //超声波数据
          if(index == 2 && rec_data[1] == 1){
            mySerial.printf("CMD|%d|%d|$",rec_data[0],ult.getDistance()*10); //读取超声波数据
          }else if(index == 3){
            if(rec_data[1] == 2){
              if(rec_data[2] == 1){
                avo_flg = 1;
              }else if(rec_data[2] == 0){
                avo_flg= 0;
              }
            }
          }
          break;

        case 5: //IMU数据
          if(index == 1){
            IMU_init(); //初始化IMU
            delay(100);
            read_angle(); //读取取PITCH、ROLL度数
            mySerial.printf("CMD|%d|%f|%f|$",rec_data[0],radianY_last,radianX_last);
          }
          break;

        case 6: //电池电量
          if(index == 1){
            mySerial.printf("CMD|%d|%d|$",rec_data[0],mechdog.readBattery()); //读取电池电量
          }
          break;
      }
    }
  }
  delay(10);
}

void IMU_init(){
  IIC1.begin(SDA1,SCL1);
  accelgyro.initialize();
  accelgyro.setFullScaleGyroRange(3); //设定角速度量程
  accelgyro.setFullScaleAccelRange(1); //设定加速度量程
}

void Project_init(){
  mechdog.MechDog_init(); //初始化MechDog
  ult.Ultrasound_init(); //初始化发光超声波模块

  xTaskCreate( //创建MechDog功能任务 
    run_task,          
    "runTask",      
    2048,           
    (void *)&obj,           
    1,              
    NULL    
  );

}

static void run_task(void *p){
  Object *self = (Object *)p;
  MechDog *dog = self->mechdog;
  UltrasoundSonar *u = self->ult;

  while(true){
    if(avo_flg == 1){ //超声波避障
      while(true){
        action_num = -1;
        actions_flg = 0;
        if(avo_flg == 0){ //停止
          dog->move(0,0);
          vTaskDelay(20);
          u->setRGB(0,0x33,0x33,0xff);
          avo_flg = -1;
          if(forward_flag == 0){
            forward_flag = 1;
            dog->transform(attitude[0],100);
          }
          vTaskDelay(1000);
          break;
        }
        if(u->getDistance() < 10){ //后退
          if(forward_flag == 1){
            forward_flag = 0;
            dog->transform(attitude[1],100);
          }
          u->setRGB(0,0xff,0x00,0x00);
          dog->move(-40,0);
          for(int i = 0; i <30; i++){
            if(avo_flg == 0){
              break;
            }
            vTaskDelay(100);
          }
        }else if(u->getDistance() < 40){ //转弯
          if(forward_flag == 0){
            forward_flag = 1;
            dog->transform(attitude[0],100);
          }
          u->setRGB(0,0xff,0xcc,0x00);
          dog->move(100,-50);
          for(int i = 0; i <30; i++){
            if(avo_flg == 0){
              break;
            }
            vTaskDelay(100);
          }
        }else{
          u->setRGB(0,0xcc,0x33,0xcc); //前进
          dog->move(120,0);
          vTaskDelay(20);
        }
      }
    }else if(action_num != -1){
      avo_flg = 0;
      actions_flg = 0;
      switch(action_num){
        case 0:
          dog->move(0,0); //停止
          action_num = -1;
          vTaskDelay(20);
          break;
        case 1:
          dog->move(90,-25); //小角度右前转
          vTaskDelay(20);
          continue;
        case 2:
          dog->move(80,-40); //大角度右前转
          vTaskDelay(20);
          continue;
        case 3:
          dog->move(120,0); //直线前进
          vTaskDelay(20);
          continue;
        case 4:
          dog->move(80,40); //大角度左转
          vTaskDelay(20);
          continue;
        case 5:
          dog->move(90,25); //小角度左转
          vTaskDelay(20);
          continue;
        case 6:
          dog->move(-40,-20); //左后退
          vTaskDelay(20);
          continue;
        case 7:
          dog->move(-40,0); //直线后退
          vTaskDelay(20);
          continue;
        case 8:
          dog->move(-40,20); //右后退
          vTaskDelay(20);
          continue;
      }
    }else if(actions_flg != 0){
      action_num = -1;
      avo_flg = 0;

      if(actions_flg == 1){
        switch(actions){
          case 1:
            dog->action_run("left_foot_kick"); //左脚踢球
            vTaskDelay(3000);
            break;
          case 2:
            dog->action_run("right_foot_kick"); //右脚踢球
            vTaskDelay(3000);
            break;
          case 3:
            dog->action_run("stand_four_legs"); //四脚站立
            vTaskDelay(3000);
            break;
          case 4:
            dog->action_run("sit_dowm"); //坐下
            vTaskDelay(3000);
            break;
          case 5:
            dog->action_run("go_prone"); //趴下
            vTaskDelay(3000);
            break;
          case 6:
            dog->action_run("stand_two_legs"); //双脚站立
            vTaskDelay(3000);
            break;
          case 7:
            dog->action_run("handshake"); //握手
            vTaskDelay(3000);
            break;
          case 8:
            dog->action_run("scrape_a_bow"); //作揖
            vTaskDelay(3000);
            break;
          case 9:
            dog->action_run("nodding_motion"); //点头
            vTaskDelay(3000);
            break;
          case 10:
            dog->action_run("boxing"); //拳击
            vTaskDelay(3000);
            break;
          case 11:
            dog->action_run("stretch_oneself"); //伸懒腰
            vTaskDelay(3000);
            break;
          case 12:
            dog->action_run("pee"); //撒尿
            vTaskDelay(3000);
            break;
          case 13:
            dog->action_run("press_up"); //俯卧撑
            vTaskDelay(3000);
            break;
          case 14:
            dog->action_run("rotation_pitch"); //转动PITCH
            vTaskDelay(3000);
            break;
          case 15:
            dog->action_run("rotation_roll"); //转动ROLL
            vTaskDelay(3000);
            break;
          case 16:
            dog->action_run("normal_attitude"); //立正
            vTaskDelay(3000);
            break;
        }
      }else{
        switch(actions){
          // case n:
          //   break;
        }
      }
      actions_flg = 0;
    }else if(homeostasis_flg == 1){ //自平衡功能
      dog->homeostasis(true);
      while(true){
        if(homeostasis_flg == 0){ //停止自平衡
          dog->homeostasis(false);
          vTaskDelay(2000);
          break;
        }else if(!(dog->read_homeostasis_status())){ //若MechDog摔倒
          mySerial.printf("CMD|%d|%d|%d|$",rec_data[0],rec_data[1],0); //摔倒反馈
          homeostasis_flg = 0;
          break;
        }
        vTaskDelay(10);
      }
    }
    vTaskDelay(10);
  }
}
