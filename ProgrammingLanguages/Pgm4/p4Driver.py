###########################################################################################
# 				p4Driver.py written by Darin Soeung
# Purpose:
#		In this assignment, you will read a file (its name is passed as a command argument)
#		that contains many text lines, each containing "@." commands.
# Command Parameters:
#    python3 p4Driver.py
#		no arguements
# Input:
#   Specific inputs are located
# Results:
# 		Prints data from an input file seperating and formatting
# 		the information into associative arrays (dictionaries).
# Returns:
#	    None
# Notes:
#       None
###########################################################################################
import pprint
from p4At import setVariable, setFormat


# Dictionaries used for setting up variables, formats, and values
variableDict = dict()
# FLOW is "YES", LM is "1", RM is "80", JUST is "LEFT", and BULLET is "o".
formatDict = {"FLOW": "YES", "LM": "1", "RM": "80", "JUST": "LEFT", "BULLET": "o"}

# Iterating through every line in the file
for line in open("p4Input.txt"):

    # Regular expression that matches the lines and separates commands and rest
    [start,variable,atString] = line.split(' ',2)

    # check if variable is "FORMAT"
    if variable== 'VAR' :
        #Call setVariable
        setVariable(atString, variableDict)
    # check if variable is "FORMAT"
    elif variable == 'FORMAT':
        #Call setFormat
        forDict = setFormat(atString, formatDict)
    # check if variable is "PRINT"
    elif variable == 'PRINT':
        if atString=='VARS\n' :
            #Print the variable names and corresponding values from the Variable associative array. Use:
            ##Double in case of \n's
            pprint.pprint(variableDict, width=30)
        elif atString == 'VARS':
            pprint.pprint(formatDict, width=30)
        ##Double in case of \n's
        elif atString=='FORMAT\n' :
            pprint.pprint(formatDict, width=30)
        elif atString=='FORMAT' :
            pprint.pprint(formatDict, width=30)
    # If none of the above print error
    else:
        print("*** Not a recognizable command, found:", start, variable, atString.rstrip())