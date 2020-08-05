#!/bin/bash

if [$# -lt 1]; then
	echo "Usage $0 <textfile>"
	echo ""
	exit 1
fi

file = $1

if [! -r "$file"]; then
	echo "error file not readable"
	echo 
	exit 1
fi

let max = 0
longest=""

while read line; do
	wcount=$(echo $line | wc -w)
	if((wcout > max)); then
		let max = wcout
		longest = $line
	fi
done < "$file"

echo "longest line = $max words, contents : "
echo $longest