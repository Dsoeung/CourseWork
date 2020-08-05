#!/bin/bash

read -p "Enter a department code and course number (dptCode(space)crsNum):" dptCode crsNum 

if [ -e data/$dptCode$crsNum.crs ]; then
  dos2unix readcont.bash
  bash readcont.bash < data/$dptCode$crsNum.crs
  read -p "Enter enrollment change amount: " enChange
  crsEnroll = crsEnroll - enChange
  echo "$crsEnroll"
else
  echo -e "ERROR: Course not found"
fi


echo "[date] Enrollment: $dptCode $crsNum $crsName changed by $enChange" >> data/queries.log