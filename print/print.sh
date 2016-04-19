#!/bin/bash

echo "Usage print.sh duplicate:[true|false] portrait:[true|false] cutter:[true|false] source.png"

duplicate=$1
portrait=$2
cutter=$3
source=$4

echo $duplicate
echo $portrait
echo $cutter
echo $source

dest=/tmp/result.png
printer=DS-RX1
media_cut=2x6_x2
media_nocut=w288h432
media=$media_nocut
orientation=landscape

convert -density 300 $source $dest

#Manage duplication

if [ "$duplicate" == "duplicate:true" ]; then
	if [ "$portrait" == "portrait:true" ]; then
		convert  -density 300 "$dest" "$dest" +append "$dest"
	else
		convert  -density 300 "$dest" "$dest" -append "$dest"
	fi
fi

#Rotate if paysage

if [ "$portrait" == "portrait:false" ]; then
	orientation=portrait
fi

#Set cutter

if [ "$cutter" == "cutter:true" ]; then
	media=$media_cut
else
	media=$media_nocut
fi

#Print

lpr -P $printer -o $orientation -o fit-to-page -o media=$media $dest


