import Hiwonder
import time
from HW_MechDog import MechDog

# 创建亮度传感器对象
adc = Hiwonder.LightSensor()
# 初始化MechDog对象
mechdog = MechDog()

# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)
# 光照亮度阈值
Intensity_threshold = 100
# 读取的亮度值
brightness = 0  

# 主函数
def main():
  global Intensity_threshold
  global brightness

  while True:
    # 读取光照强度
    brightness = adc.read()
    # 若亮度值大于阈值
    if (brightness>=Intensity_threshold):
      # 执行站立动作
      mechdog.action_run("stand_four_legs")
      time.sleep(2)
      # 行走
      mechdog.move(80,0)
      time.sleep(1)
      # 当亮度值大于阈值，则一直等待，直到小于时则跳出循环。
      while adc.read()>Intensity_threshold:
        time.sleep(0.1)
    else:
      # 停下
      mechdog.move(0,0)
      time.sleep(2)
      # 执行趴下动作组
      mechdog.action_run("go_prone")
      time.sleep(2)
      # 当亮度值小于阈值，则一直等待，直到大于时则跳出循环。
      while adc.read()<Intensity_threshold:
        time.sleep(0.1)

# 执行主函数
main()
