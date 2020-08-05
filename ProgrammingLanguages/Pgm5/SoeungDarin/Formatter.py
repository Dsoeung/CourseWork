###########################################################################################
# 				Formatter.py written by Darin Soeung
# Purpose:
#		Formats and prints strings in according to the given input file.
# Command Parameters:
#    N/A
# Input:
#    N/A
# Results:
#    Formats strings from a given input file, and is called by the driver, prints out
#       the strings in formatted form based on input files text.
# Returns:
#	    None
# Notes:
#       None
###########################################################################################
import re

# global variable representing the current working formatted output line
lines = list()
# global variable representing the current working formatted output line
fLines = list()
# •	Initialize the format dictionary (given)
formatDict = {"LM": "1", "RM": "80", "FLOW": "YES", "JUST": "LEFT",
              "BULLET": "o"}


#################################################################################################
# setVarNames (string line, dictionary varDictionary)
# Purpose:
#             Checks line by line in the given string for Variable names present in the variable
#               dictionary and replaces them with the desired key.
# Parameters:
#  I    line	                    current input line
#  I    varDictionary               Dictionary that holds every varName : value
# Returns:
#      line                         output string including the @varNames replaced with key names of
#                                   the variables
# Notes:
#      None
##################################################################################################
def setVarNames(line, varDictionary):
    # take in line by line
    linebyline = re.compile(r'^.*$')
    match = False
    # checks whole line for @varname
    if match == False:
        if linebyline.match(line):
            # check through lines with @ in them
            if '@' in line:
                #finds each @varname per line
                varFind = re.compile(r'@(\S*)')
                #add found varnames to matched list
                matched = varFind.findall(line)
                #parse through varDictionary to find matched varnames
                for key in varDictionary:
                    for i in range(0, len(matched)):
                        if key in matched[i]:
                            line = re.sub(matched[i], varDictionary[key], line)
                line = line.replace("@", "")
                return line



################################################################################################
# formatText (string line, dictionary formatDictionary, string lastLine)
# Purpose:
#       Function initializes your current working formatted output line to an empty string
#       and sets a variable to indicate at the beginning of a paragraph
# Parameters:
#  I    line	                    current input line
#  I    formatDictionary            Dictionary that holds every format : value
#  I    lastLine                    lastLine of the paragraph to check with current line
# Returns:
#		N/A
# Notes:
#
##################################################################################################
def formatText(line, formatDictionary, lastLine):
    global formatDict
    # When needing numeric values do something like this
    # int(formatDict["RM"]) - int(formatDict["LM"]) + 1
    # initialize given margins
    marginL = int(formatDictionary["LM"])
    marginR = int(formatDictionary["RM"])

    # base margins initially 1 and 80
    lmPrev = int(formatDict["LM"])
    rmPrev = int(formatDict["RM"])

    # flow bool
    flow = True

    # checks if line is a @. FORMAT line
    if "@. FORMAT" in line:
        # empties it
        line = ""
        # strips newline
        line.rstrip("\n")

    # checking justification
    if formatDictionary["JUST"] == "LEFT":
        # storing justification value
        justification = 0
    elif formatDictionary["JUST"] == "BULLET":
        # storing justification value
        justification = 1

    # if formatDict["JUST"] != "BULLET":
    # formatWidth = int(formatDict["RM"]) - int(formatDict["LM"]) + 1
    # 1 == BULLET
    if justification == 1:
        # •	Determine the formatted text width which is different for bullets.
        formatWidth = marginR - marginL + 1 - 2
    else:
        #   Example formula given
        #formatWidth = int(formatDict["RM"]) - int(formatDict["LM"]) +
        #Else Justification 0 == LEFT
        formatWidth = marginR - marginL + 1

    # codes for the previous "JUST" key stored in formatDict
    if formatDict["JUST"] == "LEFT":
        justPrev = 0
    elif formatDict["JUST"] == "BULLET":
        justPrev = 1

    # if formatDict["JUST"] != "BULLET":
    # formatWidth = int(formatDict["RM"]) - int(formatDict["LM"]) + 1
    # sets the width according to the justPrev code
    if justPrev == 1:
        widthPrev = rmPrev - lmPrev + 1 - 2
    else:
        #   Example formula given
        # formatWidth = int(formatDict["RM"]) - int(formatDict["LM"]) + 1
        widthPrev = rmPrev - lmPrev + 1

        #	If last line
    if line == '\n' or line is lastLine:
        # and flow == "YES"
        if flow == True:
            lines.append(line)
            # append and join the line
            newline = ''.join(lines)
            #   print the current working formatted output line
            printFormat(newline, line, marginL, formatWidth, justification, flow, lastLine)
            # Initialize for a new paragraph (i.e., recognize that you are on the first line of a paragraph)
            del lines[:]
    # •	Otherwise, it is a line that needs to be formatted
    elif line != '\n' and line is not lastLine and line != "":
        # o	If FLOW is "YES":
        if formatDictionary["FLOW"] == "YES":
            # print the current working formatted output line
            lines.append(line)
            flow = True
        elif formatDictionary["FLOW"] == "NO":
            flow = False
            fLines.append(line)
            #   Invoke a function to print the text.
            printFormat(fLines, lines, marginL, formatWidth, justification, flow, lastLine)
            # Initialize for a new paragraph (i.e., recognize that you are on the first line of a paragraph)
            del fLines[:]
    elif line == "":
        if formatDict["FLOW"] == "YES":
            flow = True
            newline = "".join(lines)
            printFormat(newline, line, lmPrev, widthPrev, justPrev, flow, lastLine)
            del lines[:]
    # Copy the last iteration of formatDictionary to be the Prev values.
    formatDict = dict(formatDictionary)


######################################################################################
# formatPrinter (string lines, string curLine, int leftM, int formatWidth, int justification
#                      boolean flow, boolean lastLine)
# Purpose:
#           This formats the list sent in according to the variables (flow, justification).
#           This function then prints it accordingly.
# Parameters:
#  I    lines	            String that holds the paragraph
#  I    curline             String that holds the current line
#  I    leftM               left margin space value
#  I    formatWidth         format width value (max)
#  I    justification       justification value
#  I    flow                bool that tells whether current flow is YES or NO
#  I    lastLine            last line
# Returns:
#
# Notes:
#
##################################################################################################

# 5.	Create a function to print the text.  It should
def printFormat(lines, curLine, leftM, formatWidth, justification, flow, lastLine):
    # If FLOW="YES" and the current working formatted output line is empty, there isn't anything to print.
    if flow == True and lines != "":
        # using wrap function along with the given width
        lines = wrap(lines, formatWidth)

        # line split by newline
        nLines = lines.split('\n')

        # following is for JUST = BULLET
        if justification == 1:
            if lines != " ":
                # first line in paragraph will hold the left margin + "o "
                # left - 1 to start on margin
                nLines[0] = " " * (leftM - 1) + "o " + nLines[0]
                # rest of the lines hold the left margin + 2 (to meet the first line)
                for i in range(1, len(nLines)):
                    # left - 1 to start on margin
                    nLines[i] = " " * ((leftM - 1) + 2) + nLines[i]
        else: #JUST = Left
            for i in range(0, len(nLines)):
                if justification == 0:
                    # left - 1 to start on margin
                    nLines[i] = " " * (leftM - 1) + nLines[i]

        # If curline is last then add newline
        if curLine is not lastLine:
            nLines.append(curLine)
            lines = '\n'.join(nLines)
            print(lines, end="")
        elif curLine is lastLine:
            lines = '\n'.join(nLines)
            print(lines, end="\n")
    #if FLOW == "NO"
    if flow == False:
        for i in range(0, len(lines)):
            #left - 1 to start on margin
            lines[i] = " " * (leftM - 1) + lines[i]
            print(lines[i], end="")


##########################################################################################
# wrap (string line, int width)
# Purpose:
#           formats the string along to the given width
# Parameters:
#  I    atString	        String that holds the paragraph
#  I    width               given width to format the string
# Returns:
#         Function returns the given string formatted to the given width
# Notes:
#
###############################################################################################

def wrap(atString, width):
    # get rid of that pesky newline in the middle of strings
    line = atString.replace('\n', "")
    line = " " + line
    newstring = ""
    while len(line) > width:
        # find position of nearest whitespace char to the left of "width"
        marker = width - 1
        while not line[marker].isspace():
            marker = marker - 1
        # remove line from original string and add it to the new string
        newline = line[0:marker] + "\n"
        newstring = newstring + newline
        line = " " + line[marker + 1:]
    return newstring + line + "\n"
