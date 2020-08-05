#!/usr/bin/env python3
#PYTHON#
import sys
import re
#sample input
#3 2 3 4 5 6
#5 24 4 23 56

#taken from standard input stdin
#Take #'s from std int ,first one is a position, tokenize strings and add #'s at position then take average.

sum = 0
numlines = 0 #we're averaging values
for lines in sys.stdin:
		tokens = line.split(" ") #returns list, tokenizes string split by spaces
		target = tokens[int(tokens[0])]
		sum += float(target)
		numlines += 1

print("average = " + str(sum / numlines))
		
		