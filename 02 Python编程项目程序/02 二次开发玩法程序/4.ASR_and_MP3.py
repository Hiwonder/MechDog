import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

# 初始化MechDog对象
mechdog = MechDog()
# 创建IIC2对象
i2c2 = Hiwonder_IIC.IIC(2)
# 创建语音识别模块对象
myasr = Hiwonder_IIC.ASR(i2c2)
# 创建IIC1对象
i2c1 = Hiwonder_IIC.IIC(1)
# 创建MP3模块对象
mp3 = Hiwonder_IIC.MP3(i2c1)
# 识别结果
recognize_result = 0


# 设置MechDog初始姿态
mechdog.set_default_pose()
# 设置为循环识别模式
myasr.setMode(1)
# 设置MP3音量为30
mp3.volume(30)

# 设置语音识别词条，只需要设置一次，后续的可以改为False
if True:
  myasr.addWord(0,"ni hao")
  myasr.addWord(1,"xiang qian")
  myasr.addWord(2,"xiang hou")
  myasr.addWord(3,"zuo zhuan")
  myasr.addWord(4,"you zhuan")
  myasr.addWord(5,"ting zhi")

# 延时函数，参数为延时的时间（单位：秒）
time.sleep(1)


# 主函数
def main():
  global recognize_result

  while True:
    # 获取识别结果，无结果则返回0
    recognize_result = myasr.getResult()
    # 识别到向前
    if (recognize_result==1):
      mp3.play(1)
      mp3.play()
      mechdog.move(80,0)
      time.sleep(3)
      mechdog.move(0,0)
      time.sleep(2)
    # 识别到向后
    if (recognize_result==2):
      mp3.play(2)
      mp3.play()
      mechdog.move(-80,0)
      time.sleep(3)
      mechdog.move(0,0)
      time.sleep(2)
    # 识别到左转
    if (recognize_result==3):
      mp3.play(3)
      mp3.play()
      mechdog.move(80,30)
      time.sleep(3)
      mechdog.move(0,0)
      time.sleep(2)
    # 识别到右转
    if (recognize_result==4):
      mp3.play(4)
      mp3.play()
      mechdog.move(80,-30)
      time.sleep(3)
      mechdog.move(0,0)
      time.sleep(2)
    time.sleep(0.05)

# 执行主函数
main()


