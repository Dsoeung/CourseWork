#!/bin/bash

# this reads from crs file
# the file contains:
#   first line:  dptCode dptName
#   second line: crsName
#   third line: crsSched crsStart crsEnd
#	fourth line: crsCredit
#	fifth line:	crsEnroll




read -p "Enter a department code and course number (dptCode(space)crsNum):" dptCode crsNum 

if [ -e data/$dptCode$crsNum.crs ]; then
  dos2unix readcont.bash
  bash readcont.bash < data/$dptCode$crsNum.crs
else
  echo -e "ERROR: $dptCode$crsNum course not found"
fi
