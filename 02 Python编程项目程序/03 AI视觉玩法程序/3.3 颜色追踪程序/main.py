import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

'''
  颜色追踪 -> 蓝色
'''

iic2 = Hiwonder_IIC.IIC(2)
cam = Hiwonder_IIC.ESP32S3Cam(iic2)
mechdog = MechDog()
time.sleep(5)

while True:
  # RED \ YELLOW \ GREEN \ BLUE \ BLACK
  color = cam.color_follow(cam.BLUE)
  if color:
    if color[0] == 3:
      angle = 0
      dir = 1 
      if color[1] < 60:
        angle = 25
      elif color[1] > 100:
        angle = -25
      
      if color[2] < 70:
        dir = 1
      else:
        dir = -1
      
      w = color[5] - color[3]
      h = color[6] - color[4]
      area = w*h
      print(area)
      if area > 5000:
        mechdog.move(0 , 0)
      else:
        if dir == 1:
          mechdog.move(50 , angle)
        else:
          mechdog.move(-50 , 0)
  else:
      mechdog.move(0 , 0)
  time.sleep(0.1)








