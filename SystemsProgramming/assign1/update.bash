#!/bin/bash
# this updates a crs file
# the file contains:
#   first line:  dptCode dptName
#   second line: crsName
#   third line: crsSched crsStart crsEnd
#	fourth line: crsCredit
#	fifth line:	crsEnroll
date = $(date)

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

if [ -e data/$dptCode$crsNum.crs ]; then
  rm data/$dptCode$crsNum.crs
  echo -e "$dptCode $dptName \n $crsName \n $crsSched $crsStart $crsEnd \n $crsCredit $crsEnroll" >> data/$dptCode$crsNum.crs
  
  echo  "$date UPDATED: $dptCode $crsNum $crsName" >> data/queries.log
  
  echo "Updated! : "
  
  echo "Department code: $dptCode" 
  echo "Department name: $dptName" 
  echo "Course num: $crsNum" 
  echo "Course name: $crsName" 
  echo "Course schedule: $crsSched" 
  echo "Course start date: $crsStart" 
  echo "Course end date: $crsEnd" 
  echo "Credit hours: $crsCredit" 
  echo "Students enrolled in course: $crsEnroll" 
  
else
  echo -e "ERROR: course not found"
fi
