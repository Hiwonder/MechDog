import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

'''
  颜色巡线
'''

iic2 = Hiwonder_IIC.IIC(2)
cam = Hiwonder_IIC.ESP32S3Cam(iic2)
mechdog = MechDog()
time.sleep(1)

while True:
  center1 = cam.line_follow(cam.YELLOW)
  if center1[0] == 0 and center1[1] == 0:
    pass
  elif center1[0] < 60:
    mechdog.move(50 , 25)
  elif center1[0] > 100:
    mechdog.move(50 , -25)
  else:
    mechdog.move(50 , 0)
  time.sleep(0.1)


