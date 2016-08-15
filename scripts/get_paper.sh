#!/bin/bash
RES=`sudo /opt/PhototwixMirror/bin/scripts/dnpds40 -s 2>&1 | grep Remaining | cut -d : -f 3 | sed -re 's/^[[:space:]].(.*).$/\1/'`
echo -n $RES

