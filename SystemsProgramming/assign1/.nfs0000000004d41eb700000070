#!/bin/bash

read -n4 -p "Enter the department code: " dptCode
echo
read -p "Enter department name: " dptName
echo
read -p "Enter course number: " crsNum
echo
read -p "Enter course name: " crsName
echo
read -p "Enter course schedule: " crsSched
echo
read -r -p "Enter course start date (DD/MM/YYYY): " crsStart
echo
read -r -p "Enter course end date (DD/MM/YYYY) : " crsEnd
echo
read -p "Enter course credit hours: " crsCredit
echo
read -p "Enter initial course enrollment: " crsEnroll
echo

balance=900

if [ -e $crsNum ]; then
  echo "ERROR: course already exists"
else
  echo -e "$dptCode $dptName \n $crsName \n $crsSched $crsStart $crsEnd \n $crsCredit $crsEnroll" >> data/$dptCode$crsNum.crs
  d=$(date +%Y-%m-%d)
  echo "$d CREATED: $dptName$crsNum" >> data/queries.log
fi

