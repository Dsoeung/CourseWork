  #!/bin/bash
# this reads from crs file
# the file contains:
#   first line:  dptCode dptName
#   second line: crsName
#   third line: crsSched crsStart crsEnd
#	fourth line: crsCredit
#	fifth line:	crsEnroll

  read dptCode dptName
  echo "course department: $dptCode $dptName" 
  echo "Course number: $1"
  read crsName
  echo "Course Name: $crsName"
  read crsSched crsStart crsEnd
  echo "Scheduled days: $crsSched"
  echo "Course Start: $crsStart"
  echo "Course End: $crsEnd"
  read crsCredit 
  read crsEnroll
  echo "Credit hours: $crsCredit"
  echo "Enrolled Students: $crsEnroll"
  echo "check $crsEnroll"
  if [$crsEnroll -gt 50]
  then
  
	echo "I'm gonna say it"
  
  fi
  