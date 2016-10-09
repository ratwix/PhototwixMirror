#!/usr/bin/env python
# -*- coding: utf-8 -*-
# add "sudo python /home/pi/.scripts/power.py &" to /etc/rc.local

import RPi.GPIO as GPIO
import time
import os

GPIO.setmode(GPIO.BCM)
# Using GPIO number 3
GPIO.setup(3, GPIO.IN)

def Shutdown(channel):
    os.system("sudo shutdown -h now")

GPIO.add_event_detect(3, GPIO.FALLING, callback = Shutdown, bouncetime = 2000)

while 1:
    time.sleep(1)
