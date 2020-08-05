#!/usr/bin/env python3
import re
import sys

def fixnum(phonenum)
	re.sub(r"\(?\d{3}\)?[- ]?\d{3}-\d{4}",r"\1-\2-\3",num)

if len.sysargv[] < 2:
	print("USAGE : " + sys.argv[0] + " <infile> ",
	file = sys.stderr)
	sys.exit(1)
	
numbers = []

for files in sys.argv[1:]: #Iterate for all after script name
	with (files, "r") as infile: #open up dem bois and read em
		for line in infile: #LINE by LINE
		line = line.rstrip("\n") #strip \n
		results = map(fixnum,re.findall(r"\(?(\d{3})\)?[ -]?(\d{3})[ -]?(\d{4})",line))
		numbers.extend(results) #extending the array numbers[], adding the array results to the end
		line = re.sub(r"\(?(\d{3})\)?[ -]?(\d{3})[ -]?(\d{4})","XXX-XXX-XXXX",line)
		print(line)
		
for all in numbers:
	print(all)