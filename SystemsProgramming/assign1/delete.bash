#!/bin/bash

date = $(date)

read -p "Enter a department code and course number (dptCode(space)crsNum):" dptCode crsNum 

if [ -e data/$dptCode$crsNum.crs ]; then
  rm rm data/$dptCode$crsNum.crs
  echo -e "$dptCode$crsNum was successfully deleted"
  echo "$date DELETED: $dptCode$crsNum" >> data/queries.log
else
  echo -e "ERROR: apartment not found"
fi

