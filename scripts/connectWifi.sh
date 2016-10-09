#!/bin/bash

cp /etc/wpa_supplicant/wpa_supplicant.conf.base /tmp/wpa_supplicant.conf

echo "network={" >> /tmp/wpa_supplicant.conf
echo "  ssid=\"$1\"" >> /tmp/wpa_supplicant.conf
if test -z "$2"
then
# No Password
    if test -z "$1"
    then
        echo "  psk=\"\"" >> /tmp/wpa_supplicant.conf
    else
        echo "  proto=RSN"  >> /tmp/wpa_supplicant.conf
        echo "  key_mgmt=NONE"  >> /tmp/wpa_supplicant.conf
    fi
else
    echo "  psk=\"$2\"" >> /tmp/wpa_supplicant.conf
fi


echo "}" >> /tmp/wpa_supplicant.conf

cp /tmp/wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant.conf

sudo ifdown wlan0
sudo ifup wlan0

