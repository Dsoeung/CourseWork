###########################################################################################
# 				p5Driver.py written by Darin Soeung
# Purpose:
#		In this assignment, you will read a file (its name is passed as a command argument)
#		that contains many text lines containing "@." commands.
# Command Parameters:
#    python3 p5Driver.py <filename>
#
# Input:
#    an input file denoted by <filename> above
# Results:
# 		Prints data from an input file separating and formatting
# 		the information into associative arrays (dictionaries).
# Returns:
#	    None
# Notes:
#       None
###########################################################################################

from p4At import setVariable, setFormat
from Formatter import setVarNames, formatText
import re
import sys

# You must be able to invoke your  code using:  python3  p5Driver.py inputFilename
# so we need variable/argument input file instead of opening an input.txt
inputFile = open(sys.argv[1])
lines = inputFile.readlines()
# grab the last line of the file
lastLine = lines[-1]

# Dictionaries used for setting up variables, formats, and values
variableDict = dict()
# FLOW is "YES", LM is "1", RM is "80", JUST is "LEFT", and BULLET is "o".
formatDict = {"FLOW": "YES", "LM": "1", "RM": "80", "JUST": "LEFT", "BULLET": "o"}
# Iterating through every line in the file
for line in lines:
    #initialize matched
    matched = False

    # Finds all format lines
    #@. FORMAT ETC
    format = re.compile(r'@\.\s*(\S*)\s(.*)$')
    # group 2 = dictionary line ie. LM = xx RM = xx etc.

    #if matched, or has the FORMAT line in it
    if format.match(line):
        #matched is true
        matched = True
        match = format.search(line)

    if matched == True:
        # Matches "FORMAT" strings
        if (match.group(1) == "FORMAT"):
            # Calls setFormat
            setFormat(match.group(2), formatDict)
            # calls formatText
            formatText(line, formatDict, lastLine)

        # Matches "VAR" strings
        elif (match.group(1) == "VAR"):
            # Calls setVariable
            setVariable(match.group(2), variableDict)

        # check if variable is "PRINT"
        elif (match.group(1) == "PRINT"):
            if match.group(2) == "VARS\n":
                print(variableDict)
            elif match.group(2) == "VARS":
                print(variableDict)
            elif match.group(2) == "FORMAT":
                print(formatDict)
            elif match.group(2) == "FORMAT\n":
                print(formatDict)
    else:
        # calls setVarName
        varLines = setVarNames(line, variableDict)
        # checks if string is None, formatLine takes current iterating line instead
        if varLines != None:
            formatText(varLines, formatDict, lastLine)
        elif varLines == None or line is lastLine:
            formatText(line, formatDict, lastLine)