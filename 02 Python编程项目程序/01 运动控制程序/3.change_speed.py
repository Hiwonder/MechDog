import Hiwonder
import time
from HW_MechDog import MechDog

# 按键按下标志位
enter_flag = 0
# 速度初始值
speed = 40

# 初始化MechDog对象
mechdog = MechDog()
# 按键对象
button1 = Hiwonder.Button(1)

# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)


# 主函数
def main():
  global enter_flag
  while True:
    if (enter_flag==1):
      if (speed==40):
        mechdog.move(speed,0)
        time.sleep(10)
        speed = 60
      elif (speed==60):
        mechdog.move(speed,0)
        time.sleep(10)
        speed = 80
      else:
        mechdog.move(speed,0)
        time.sleep(10)
        speed = 40
      # 停止
      mechdog.move(0,0)
      enter_flag = 0
      
    time.sleep(0.05)

# 按键短按调用的函数
def on_button1_clicked():
  global enter_flag
  enter_flag = 1

# 注册按键短按调用函数
button1.Clicked(on_button1_clicked)

# 执行主函数
main()

