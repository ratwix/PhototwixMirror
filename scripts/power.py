# Copy this script in /home/pi/.scripts
# Connect button on PIN GPIO 3 (5 in Rpi 3) and GND (9 in Rpi 3)
# Add "sudo python /home/pi/Scripts/power.py &" to /etc/rc.local/etc/rc.local before exit 0

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
