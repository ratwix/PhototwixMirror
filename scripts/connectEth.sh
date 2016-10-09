#!/bin/bash

sudo cp /etc/dhcpcd.conf.base /tmp/dhcpcd.conf

sudo echo "interface eth0" >> /tmp/dhcpcd.conf
sudo echo "  static ip_address="$1 >> /tmp/dhcpcd.conf
# sudo echo "  static routers="$2 >> /tmp/dhcpcd.conf
# sudo echo "  static domain_name_servers="$2 >> /tmp/dhcpcd.conf
# sudo echo "  static domain_search =" >> /tmp/dhcpcd.conf

sudo cp /tmp/dhcpcd.conf /etc/dhcpcd.conf
sudo reboot
