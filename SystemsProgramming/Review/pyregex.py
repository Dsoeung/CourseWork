#!/usr/bin/env python3
#PYTHON#
import sys
import re

#take in regex arg[1] and find matches in given multiple files arg[2..n]

#argv = script name, regex, f1
#argv = 0          , 1    , 2 but of length 3
#must have at LEAST, script name, regex, a file(s)

if len(sys.argv) < 3:
	print("Usage: " + sys.argv[0] + "<regex> <file1> [file2...fileN]", file=sys.stderr)
	sys.exit(1)

sys.argv.pop(0) # popping script name for future loop help
regex = sys.argv.pop(0) #regex initialized in program

count = 0
for filename in sys.argv:

	with open (filename, "r") as infile:
		linenum = 0
		filecount = 0
		for line in infile
		line += 1
		result = re.findall(regex,line)
		if result:
			count+= len(result)
			filecount += len(result)
		if filecount > 0:
			print(filename + " = " + str(filecount))
			
print()
print("total count = " + str(count))