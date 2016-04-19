#!/bin/bash
RES=`sudo /home/charles/QT/selphy_print/selphy_print/dnpds40 -s 2>&1 | grep Remaining | cut -d : -f 3 | sed -re 's/^[[:space:]].(.*).$/\1/'`
echo -n $RES

