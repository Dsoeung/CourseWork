  #!/bin/bash
# this reads from crs file
# the file contains:
#   first line:  dptCode dptName
#   second line: crsName
#   third line: crsSched crsStart crsEnd
#	fourth line: crsCredit
#	fifth line:	crsEnroll

  #Course dep
  read dptCode dptName
  echo "$dptCode $dptName" 
  #Course Num
  echo "$1"
  #Course Name
  read crsName
  echo "$crsName"
  read crsSched crsStart crsEnd
  #crs Schedule
  echo "$crsSched"
  #crs Start
  echo "$crsStart"
  #crs End
  echo "$crsEnd"
  read crsCredit 
  read crsEnroll
  #crs Credit
  echo "$crsCredit"
  #crs Enroll
  echo "$crsEnroll"
  
  if [ "$crsEnroll" -gt 50 ]
  then
  #do template from here?
  #use sed/awk to replace values into a given template.
	#echo "I'm gonna say it: $1 or $0"
  fi
  