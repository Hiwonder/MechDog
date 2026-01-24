import Hiwonder
import time
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()


# 设置MechDog初始姿态
mechdog.set_default_pose()
# 延时函数，参数为延时的时间（单位：秒）
time.sleep(2)
# 将步幅设置的很小（1毫米），近似于原地踏步
mechdog.move(1,0)
time.sleep(10)
# 停止
mechdog.move(0,0)
