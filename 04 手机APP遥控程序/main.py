

# Hiwonder MechDog
# MicroPython - APP Bluetooth control

import Hiwonder
import time
import Hiwonder_IIC
from Hiwonder_BLE import BLE
from HW_MechDog import MechDog
import machine

_BLE_REC_DATA = 0
_COMMAND = 0
_SEND_DATA = 0
_DATA = 0
_distance = 0
_SONER_DISTANCE = 0
_self_balancing_flag = 0
_RUN_STEP = 0
_obstacle_avoidance_flag = 0
_Pitch_angle = 0
_Roll_angle = 0
_ACTION_TYPE = 0
_ACTION_NUM = 0
_RUN_DIR = 0
_REC_PARSE_VALUE = []
_High_mm = 0

mac = machine.unique_id()
ble = BLE(BLE.MODE_BLE_SLAVE,"MechDog_{:02X}".format(mac[5]))
mechdog = MechDog()
i2c1 = Hiwonder_IIC.IIC(1)
i2csonar = Hiwonder_IIC.I2CSonar(i2c1)

time.sleep(1)

def start_main():
  global ble
  global _BLE_REC_DATA
  global _COMMAND
  global _SEND_DATA
  global _DATA
  global _distance
  global _SONER_DISTANCE
  global _self_balancing_flag
  global _RUN_STEP
  global _obstacle_avoidance_flag
  global _Pitch_angle
  global _Roll_angle
  global mechdog
  global _ACTION_TYPE
  global _ACTION_NUM
  global _RUN_DIR
  global _REC_PARSE_VALUE
  global _High_mm
  
  dir_flag = 1
  while True:
    if ble.is_connected():
      if ble.contains_data("CMD"):
        _BLE_REC_DATA = ble.read_uart_cmd()
        if not _BLE_REC_DATA:
          continue
        _REC_PARSE_VALUE = ble.parse_uart_cmd(_BLE_REC_DATA)
        _COMMAND = _REC_PARSE_VALUE[0]
        _COMMAND = int(_COMMAND)
        if (_COMMAND==6):
          _SEND_DATA = "CMD|6|{}|$".format(Hiwonder.Battery_power())
          ble.send_data(_SEND_DATA)
          continue
        if (_COMMAND==4):
          _DATA = int(_REC_PARSE_VALUE[1])
          if (_DATA==1):
            _distance = round((_SONER_DISTANCE*10))
            if (_distance>5000):
              _distance = 5000
            _SEND_DATA = "CMD|4|{}|$".format(_distance)
            ble.send_data(_SEND_DATA)
          elif ((_DATA==2) and (_self_balancing_flag==0)):
            if ((int(_REC_PARSE_VALUE[2]))==1):
              mechdog.set_default_pose()
              _Pitch_angle = 0
              _Roll_angle = 0
              _High_mm = 0
              time.sleep(1)
              _RUN_STEP = 41
            else:
              _RUN_STEP = 40
          elif (_DATA==3):
            i2csonar.setRGB(0,(int(_REC_PARSE_VALUE[2])),(int(_REC_PARSE_VALUE[3])),(int(_REC_PARSE_VALUE[4])))
          continue
        if ((_COMMAND==1) and (_obstacle_avoidance_flag==0)):
          _DATA = int(_REC_PARSE_VALUE[1])
          if (_DATA==3):
            _DATA = int(_REC_PARSE_VALUE[2])
            if (_DATA==1):
              mechdog.set_default_pose()
              _Pitch_angle = 0
              _Roll_angle = 0
              _High_mm = 0
              time.sleep(1)
              _RUN_STEP = 131
            else:
              _RUN_STEP = 130
          if ((_obstacle_avoidance_flag==0) and (_self_balancing_flag==0)):
            if (_DATA==1):
              _DATA = int(_REC_PARSE_VALUE[2])
              if (_DATA==1):
                if (_Roll_angle<17):
                  _Roll_angle+=1
                  mechdog.transform([0, 0, 0], [-1, 0, 0], 80)
              else:
                if (_Roll_angle>-17):
                  _Roll_angle-=1
                  mechdog.transform([0, 0, 0], [1, 0, 0], 80)
            if (_DATA==2):
              _DATA = int(_REC_PARSE_VALUE[2])
              if (_DATA==1):
                if (_Pitch_angle<17):
                  _Pitch_angle+=1
                  mechdog.transform([0, 0, 0], [0, 1, 0], 80)
              else:
                if (_Pitch_angle>-17):
                  _Pitch_angle-=1
                  mechdog.transform([0, 0, 0], [0, -1, 0], 80)
            if (_DATA==4):
              _DATA = int(_REC_PARSE_VALUE[2])
              if (_DATA==1):
                if (_High_mm < 15):
                  _High_mm += 1
                  mechdog.transform([0, 0, 1], [0, 0, 0], 80)
              else:
                if _High_mm > -25:
                  _High_mm -= 1
                  mechdog.transform([0, 0, -1], [0, 0, 0], 80)
            if (_DATA==5):
              mechdog.set_default_pose()
              _Pitch_angle = 0
              _Roll_angle = 0
              _High_mm = 0
              time.sleep(1)
            continue
        if (_COMMAND==2) and (_obstacle_avoidance_flag==0) and (_self_balancing_flag==0):
          _RUN_STEP = 2
          _ACTION_TYPE = int(_REC_PARSE_VALUE[1])
          _ACTION_NUM = int(_REC_PARSE_VALUE[2])
        if (_COMMAND==3) and (_obstacle_avoidance_flag==0) and (_self_balancing_flag==0):
          _RUN_STEP = 3
          _RUN_DIR = int(_REC_PARSE_VALUE[1])
          if _RUN_DIR < 6:
            if dir_flag != 1:
              dir_flag = 1
              mechdog.transform([10 , 0 , 0] , [0 , 0 , 0] , 100)
          else:
            if dir_flag != -1:
              dir_flag = -1
              mechdog.transform([-10 , 0 , 0] , [0 , 0 , 0] , 100)
      else:
        time.sleep(0.03)


def start_main1():
  global ble
  global _SONER_DISTANCE
  global _self_balancing_flag
  global _RUN_STEP
  global _obstacle_avoidance_flag
  global mechdog
  global _ACTION_TYPE
  global _ACTION_NUM
  global _RUN_DIR
  global i2csonar
  global _Pitch_angle
  global _Roll_angle
  global _High_mm

  step = 0
  while True:
    if (step==0):
      step = _RUN_STEP
      _RUN_STEP = 0
      time.sleep(0.05)
    else:
      if (step==41):
        _obstacle_avoidance_flag = 1
        forward_flag = 1
        while True:
          if ((_RUN_STEP==40) or (_obstacle_avoidance_flag==0)):
            _obstacle_avoidance_flag = 0
            mechdog.move(0,0)
            i2csonar.setRGB(0,0x33,0x33,0xff)
            if forward_flag == 0:
                forward_flag = 1
                mechdog.transform([10 , 0 , 0] , [0 , 0 , 0] , 100)
            time.sleep(1)
            break
          if (_SONER_DISTANCE<10):
            i2csonar.setRGB(0,0xff,0x00,0x00)
            if forward_flag == 1:
              forward_flag = 0
              mechdog.transform([-10 , 0 , 0] , [0 , 0 , 0] , 100)
            mechdog.move(-40,0)
            for count in range(30):
              if (_RUN_STEP==40):
                break
              time.sleep(0.1)
          else:
            if forward_flag == 0:
                forward_flag = 1
                mechdog.transform([10 , 0 , 0] , [0 , 0 , 0] , 100)
            if (_SONER_DISTANCE<40):
              i2csonar.setRGB(0,0xff,0xcc,0x00)
              mechdog.move(80,-50)
              for count in range(50):
                if (_RUN_STEP==40):
                  break
                time.sleep(0.1)
            else:
              i2csonar.setRGB(0,0xcc,0x33,0xcc)
              mechdog.move(120,0)
          time.sleep(0.02)
      if (step==131):
        _self_balancing_flag = 1
        mechdog.homeostasis(True)
        time.sleep(2)
        while True:
          if (_RUN_STEP==130):
            _self_balancing_flag = 0
            mechdog.homeostasis(False)
            time.sleep(2)
            break
          if not (mechdog.read_homeostasis_status()):
            ble.send_data("CMD|1|3|0|$")
            _self_balancing_flag = 0
            break
      if (step==2):
        if (_ACTION_TYPE==1):
          mechdog.set_default_pose(duration = 500)
          _Pitch_angle = 0
          _Roll_angle = 0
          _High_mm = 0
          time.sleep(1)
          dong_zuo_zu_yun_xing(_ACTION_NUM)
        else:
          mechdog.set_default_pose(duration = 500)
          _Pitch_angle = 0
          _Roll_angle = 0
          _High_mm = 0
          time.sleep(1)
          mechdog.action_run(str(_ACTION_NUM))
      if (step==3):
        while True:
          if (_RUN_DIR==0):
            mechdog.move(0,0)
            break
          elif (_RUN_DIR==1):
            mechdog.move(80,-40)
            continue
          elif (_RUN_DIR==2):
            mechdog.move(90,-25)
            continue
          elif (_RUN_DIR==3):
            mechdog.move(120,0)
            continue
          elif (_RUN_DIR==4):
            mechdog.move(90,25)
            continue
          elif (_RUN_DIR==5):
            mechdog.move(80,40)
            continue
          elif (_RUN_DIR==6):
            mechdog.move(-40,-20)
            continue
          elif (_RUN_DIR==7):
            mechdog.move(-40,0)
            continue
          elif (_RUN_DIR==8):
            mechdog.move(-40,20)
            continue
      step = 0

def dong_zuo_zu_yun_xing(dong_zuo):
  global mechdog

  if (dong_zuo==1):
    mechdog.action_run("left_foot_kick")
    time.sleep(3)
    return
  if (dong_zuo==2):
    mechdog.action_run("right_foot_kick")
    time.sleep(3)
    return
  if (dong_zuo==3):
    mechdog.action_run("stand_four_legs")
    time.sleep(2)
    return
  if (dong_zuo==4):
    mechdog.action_run("sit_dowm")
    time.sleep(2)
    return
  if (dong_zuo==5):
    mechdog.action_run("go_prone")
    time.sleep(2)
    return
  if (dong_zuo==6):
    mechdog.action_run("stand_two_legs")
    time.sleep(4)
    return
  if (dong_zuo==7):
    mechdog.action_run("handshake")
    time.sleep(4)
    return
  if (dong_zuo==8):
    mechdog.action_run("scrape_a_bow")
    time.sleep(4)
    return
  if (dong_zuo==9):
    mechdog.action_run("nodding_motion")
    time.sleep(2)
    return
  if (dong_zuo==10):
    mechdog.action_run("boxing")
    time.sleep(2)
    return
  if (dong_zuo==11):
    mechdog.action_run("stretch_oneself")
    time.sleep(2)
    return
  if (dong_zuo==12):
    mechdog.action_run("pee")
    time.sleep(2)
    return
  if (dong_zuo==13):
    mechdog.action_run("press_up")
    time.sleep(2)
    return
  if (dong_zuo==14):
    mechdog.action_run("rotation_pitch")
    time.sleep(2)
    return
  if (dong_zuo==15):
    mechdog.action_run("rotation_roll")
    time.sleep(2)
    return


def start_main2():
  global _SONER_DISTANCE
  global i2csonar

  time.sleep(2)
  while True:
    _SONER_DISTANCE = i2csonar.getDistance()
    time.sleep(0.08)

Hiwonder.startMain(start_main)
Hiwonder.startMain(start_main1)
Hiwonder.startMain(start_main2)




