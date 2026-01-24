import Hiwonder
import time
import Hiwonder_IIC
from HW_MechDog import MechDog

'''
  颜色识别
'''

iic2 = Hiwonder_IIC.IIC(2)
cam = Hiwonder_IIC.ESP32S3Cam(iic2)

i2c1 = Hiwonder_IIC.IIC(1)
i2csonar = Hiwonder_IIC.I2CSonar(i2c1)

mechdog = MechDog()
time.sleep(5)

while True:
  color_list = cam.color_recognition()
  print(color_list)
  if cam.RED in color_list: # red
    i2csonar.setRGB(0 , 250 , 0 , 0)
  time.sleep(0.01)

  if cam.GREEN in color_list: # green
    i2csonar.setRGB(0 , 0 , 250 , 0)
  time.sleep(0.1)

  if cam.BLUE in color_list: # blue
    i2csonar.setRGB(0 , 0 , 0 , 250)
  time.sleep(0.01)





