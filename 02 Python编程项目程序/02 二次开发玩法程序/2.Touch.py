import Hiwonder
import time
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()
# 按键按下标志位
enter_flag = 0
# 动作组编号
action_num = 0
# 创建按键对象
button2 = Hiwonder.Button(2)


# 设置MechDog初始姿态
mechdog.set_default_pose()
# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)

# 主函数
def main():
  global enter_flag
  global action_num

  while True:
    # 若按键按下
    if (enter_flag==1):
      if (action_num==1):
        # 执行默认动作组：坐下
        mechdog.action_run("sit_dowm")
        time.sleep(1.5)
        action_num+=1
      elif (action_num==2):
        # 执行默认动作组：趴下
        mechdog.action_run("go_prone")
        time.sleep(1.5)
        action_num+=1
      else:
        # 执行默认动作组：站立
        mechdog.action_run("stand_four_legs")
        time.sleep(1.5)
        action_num = 1
      # 清除按下标志位
      enter_flag = 0
    else:
      time.sleep(0.2)

# 按键短按调用函数
def on_extbutton_clicked():
  global enter_flag
  enter_flag = 1


# 注册按键短按调用函数
button2.Clicked(on_extbutton_clicked)

# 执行主函数
main()
