import Hiwonder
import time
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()

# 设置MechDog初始姿态
mechdog.set_default_pose()
# 延时函数，参数为延时的时间（单位：秒）
time.sleep(2)

# 主函数
def main():
  global mechdog
  
  mechdog.action_run("left_foot_kick")
  time.sleep(3)
  
# 执行主函数
main()
