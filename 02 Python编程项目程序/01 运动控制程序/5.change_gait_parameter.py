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
  # 设置步态参数，参数内容如下：
  # 参数1：脚尖离地的时间；
  # 参数2：脚尖接触地面的时间；
  # 参数3：抬腿的高度。
  mechdog.set_gait_params(150,500,40)
  mechdog.move(50,0)
  time.sleep(5)
  mechdog.move(0,0)
  time.sleep(3)
  mechdog.set_gait_params(100,300,20)
  mechdog.move(50,0)
  time.sleep(5)
  mechdog.move(0,0)
  time.sleep(3)

# 执行主函数
main()
