import Hiwonder
import time
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()


# 主函数
def main():
  # 设置MechDog初始姿态
  mechdog.set_default_pose()
  # 延时函数，参数为延时的时间（单位：秒）
  time.sleep(2)
  mechdog.move(50,-16)
  time.sleep(5)
  mechdog.transform([0, 0, 1 * 20], [0, 0, 0], 1000)
  time.sleep(5)
  mechdog.transform([0, 0, -1 * 30], [0, 0, 0], 1000)
  time.sleep(5)
  mechdog.move(0,0)
  time.sleep(2)
  # 设置MechDog初始姿态
  mechdog.set_default_pose()

# 执行主函数
main()

