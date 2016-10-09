#!/bin/sh -e
# Export pin for button and flash
# add "sudo sh /opt/PhototwixMirror/bin/scripts/export_gpio.sh &" to /etc/rc.local

#gpio export 2 in
gpio export 2 in
gpio export 4 out
