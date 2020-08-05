#!/bin/bash

#error check for parameters
if [[ "$#" -ne 4 ]]; then
	echo "ERROR: illegal number of parameters"
	exit 2
fi

#error check for dating format
if [[ $3 -ne $(date +"%m/%d/%Y") ]]; then
	echo "ERROR: date is not in the correct format MM/DD/YYYY"
	exit 2
fi

mkdir -p $4
for files in $1/*.crs; do
	line1=$(awk '{if(NR==1) print $0}' $files)
	line2=$(awk '{if(NR==2) print $0}' $files)
	line3=$(awk '{if(NR==3) print $0}' $files)
	line4=$(awk '{if(NR==4) print $0}' $files)
	line5=$(awk '{if(NR==5) print $0}' $files)
	dptCode=$(echo $line1 | cut -d ' ' -f1)
	dptName=$(echo $line1 | cut -d ' ' -f2)
	crsName=$line2
	crsSched=$(echo $line3 | cut -d ' ' -f1)
	crsStart=$(echo $line3 | cut -d ' ' -f2)
	crsEnd=$(echo $line3 | cut -d ' ' -f3)
	crdHrs=$line4
	stuNum=$line5
	crsNum="${files:0:-4}"
	crsNum="${crsNum: -4}"
	if [[ $stuNum -gt 50 ]]; then
		filename=${dptCode}${crsNum}
	fi
	> $4/${filename}.warn
	while IFS= read -r line; do
		line=$(sed "s@\\[\\[dept_code\\]\\]@${dptCode}@g" <<< "$line")
		line=$(sed "s@\\[\\[dept_name\\]\\]@${dptName}@g" <<< "$line")
		line=$(sed "s@\\[\\[course_name\\]\\]@${crsName}@g" <<< "$line")
		line=$(sed "s@\\[\\[course_start\\]\\]@${crsStart}@g" <<< "$line")
		line=$(sed "s@\\[\\[course_end\\]\\]@${crsEnd}@g" <<< "$line")
		line=$(sed "s@\\[\\[credit_hours\\]\\]@${crdHrs}@g" <<< "$line")
		line=$(sed "s@\\[\\[num_students\\]\\]@${stuNum}@g" <<< "$line")
		line=$(sed "s@\\[\\[course_num\\]\\]@${crsNum}@g" <<< "$line")
		line=$(sed "s@\\[\\[date\\]\\]@${3}@g" <<< "$line")
		echo "$line">>$4/${filename}.warn
	done < "$2"
done
exit 0