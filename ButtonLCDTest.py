#ButtonLCDTest.py

import mraa
import time
import sys
import math
import os

currentDir = os.getcwd();

#U3800A Buttons
Button1 = mraa.Gpio(33)
Button1.dir(mraa.DIR_IN)

Button2 = mraa.Gpio(46)
Button2.dir(mraa.DIR_IN)

Button3 = mraa.Gpio(32)
Button3.dir(mraa.DIR_IN)

Button4 = mraa.Gpio(45)
Button4.dir(mraa.DIR_IN)


while(1):

    if Button1.read() == 0:
        message = " Test Button_1"
        message_final = currentDir + message
        os.system("/home/root/lcdtest/lcddisplay TestButton1")
    if Button2.read() == 0:
        message = " Test Button_2"
        message_final = currentDir + message
        os.system("/home/root/lcdtest/lcddisplay TestButton2")
    if Button3.read() == 0:
        message = " Test Button_3"
        message_final = currentDir + message
        os.system("/home/root/lcdtest/lcddisplay TestButton3")
    if Button4.read() == 0:
        message = " Test Button_4"
        message_final = currentDir + message
        os.system("/home/root/lcdtest/lcddisplay TestButton4")
