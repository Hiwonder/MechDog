import Hiwonder
import time
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()

# 主函数
def main():
  # 延时函数，参数为延时的时间（单位：秒）
  time.sleep(2)
  # move()函数
  # 参数1：步幅（单位mm）（正值为向前，负值为向后）；
  # 参数2：转弯角度（单位：度），正值为左转，负值为右转
  mechdog.move(50,20) # 左转
  time.sleep(10)
  mechdog.move(0,0)
  time.sleep(2)
  mechdog.move(50,-20) # 右转
  time.sleep(5)
  # 停止
  mechdog.move(0,0)
  time.sleep(2)

# 执行主函数
main()

