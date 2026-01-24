import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()
# 创建IIC1对象
i2c1 = Hiwonder_IIC.IIC(1)
# 创建发光超声波对象
i2csonar = Hiwonder_IIC.I2CSonar(i2c1)
# 创建蜂鸣器对象
beep = Hiwonder.Buzzer()

# 超声波测量距离
distance = 0

# 设置MechDog初始姿态
mechdog.set_default_pose()
# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)


# 主函数
def main():
  global distance

  while True:
    # 获取发光超声波测量的距离
    distance = i2csonar.getDistance()
    # 若距离小于10cm
    if (distance<10):
      # 发光超声波设置颜色函数
      # 参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
      # 参数2、3、4：对应红、绿、蓝3种颜色值
      i2csonar.setRGB(0,255,0,0) # 设置为红色
    else:
      if (distance>50):
        i2csonar.setRGB(0,0,255,0) # 设置为绿色
      else:
        i2csonar.setRGB(0,(250-((round(distance))*5)),((round(distance))*5),0) # 根据距离设置红、绿2种颜色
    time.sleep(0.1)

# 蜂鸣器鸣响函数
def start_main1():
  global distance

  while True:
    # 当距离小于50cm时，根据距离鸣响
    if (distance<=50):
      beep.playTone(800,100,True)
      time.sleep((distance/50))
    else:
      time.sleep(50)

# 注册蜂鸣器鸣响线程
Hiwonder.startMain(start_main1)
# 执行主函数
main()
