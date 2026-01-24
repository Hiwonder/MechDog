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
  # 姿态变换函数
  # 参数1：平移身体（x,y,z轴）
  # 参数2：转动身体（绕x,y,z轴转动）
  # 参数3：变换的时间
  mechdog.transform([0, 0, 0], [1 * 15, 0, 0], 500) # 绕x轴选择（即Pitch）
  time.sleep(2)
  mechdog.transform([0, 0, 0], [-1 * 30, 0, 0], 1000)
  time.sleep(2)
  # 设置MechDog初始姿态
  mechdog.set_default_pose()
  time.sleep(2)
  mechdog.transform([0, 0, 0], [0, 1 * 15, 0], 500) # 绕y轴旋转（即Roll）
  time.sleep(2)
  mechdog.transform([0, 0, 0], [0, -1 * 30, 0], 1000)
  time.sleep(2)
  # 设置MechDog初始姿态
  mechdog.set_default_pose()
  time.sleep(2)

# 执行主函数
main()
