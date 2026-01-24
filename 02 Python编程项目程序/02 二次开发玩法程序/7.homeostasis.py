import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()

#初始化发光超声波对象
i2c1 = Hiwonder_IIC.IIC(1)
i2csonar = Hiwonder_IIC.I2CSonar(i2c1)
# 创建蜂鸣器对象
beep = Hiwonder.Buzzer()

# 设置MechDog初始姿态
mechdog.set_default_pose()
time.sleep(1)


# 主函数
def main():
  # 发光超声波设置颜色函数
  # 参数1：设置的灯，0为2个灯都设置，1为设置灯1,2为设置灯2；
  # 参数2、3、4：对应红、绿、蓝3种颜色值
  i2csonar.setRGB(0,0xff,0xcc,0x33)
  # 开启自平衡状态
  mechdog.homeostasis(True)
  time.sleep(2)
  # 检测是否还在自平衡状态，当退出自平衡状态时即退出该循环
  while mechdog.read_homeostasis_status():
    time.sleep(0.1)
  i2csonar.setRGB(0,0x33,0x33,0xff)
  beep.playTone(800,100,True)
  
# 执行主函数
main()

