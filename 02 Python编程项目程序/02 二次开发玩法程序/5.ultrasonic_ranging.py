import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()
# 创建点阵对象
tm = Hiwonder.Digitaltube()
# 创建IIC1对象
i2c1 = Hiwonder_IIC.IIC(1)
# 创建发光超声波对象
i2csonar = Hiwonder_IIC.I2CSonar(i2c1)

# 设置MechDog初始姿态
mechdog.set_default_pose()
# 设置点阵亮度为4
tm.setBrightness(4)
# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)

# 超声波测量距离
distance = 0

# 主函数
def main():
  global distance

  while True:
    distance = i2csonar.getDistance()
    tm.showNum(distance)
    if (distance<15):
      # 发光超声波设置颜色函数
      # 参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
      # 参数2、3、4：对应红、绿、蓝3种颜色值
      i2csonar.setRGB(0,0xff,0x00,0x00) # 设置为红色
    else:
      if (distance>40):
        i2csonar.setRGB(0,0x00,0x00,0x99) # 设置为蓝色
      else:
        i2csonar.setRGB(0,0xfd,0xd0,0x00) # 设置为黄色
    time.sleep(0.1)

# 执行主函数
main()
