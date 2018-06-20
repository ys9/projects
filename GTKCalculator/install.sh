#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

tar -xvf simpleCalc*.tar.gz > /dev/null
rm simpleCalc*.tar.gz
mv simpleCalc /usr/bin
mv calc.ico /usr/share/pixmaps
mv simple_calculator.desktop /usr/share/applications
rm install.sh