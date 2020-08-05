import re


################ setVariable #################################################################
# setVariable (string atString, dictionary varDictionary)
# Purpose:
#		Reads the atString which contains a varName (@varName) and value (=value)
#		then puts them into the given variable dicionary (varDictionary) that was passed in.
# Parameters:
#  I    atString				the remainder of the string after the VAR or FORMAT key word.
#  I    varDictionary			Variable dictionary, varname : value
# Returns:
#		None
# Notes :
#       for the regex pattern I used \S for any non-whitespace character to grab the . for dr.
#       I kept it this way in case there is a case where other symbols are used.
###############################################################################################
def setVariable(atString, varDictionary):
    # pattern to check if atString is the correct format
    # @varName=value
    # group 1 = varname
    # group 2 = value
    errCheck = re.compile(r'@(\S*)=(\".*\"|\S*)')

    # Checks if atString matches the regular expression to continue or print error
    if errCheck.match(atString):
        check = errCheck.search(atString)
    else:
        print("*** Invalid format, found:", atString.rstrip())
        return None

    varName = check.group(1)
    value = check.group(2)

    # Must remove double quotes
    if '"' in value:
        removeDq = re.compile(r'\"(.*)\"')
        value = removeDq.search(value)
        varDictionary[varName] = value.group(1)
    else:
        varDictionary[varName] = value


###############################################################################################
# setFormat (string atString, dictionary formatDictionary)
# Purpose:
#		Reads the atString which contains a format tag and value (=value)
#		then puts them into the given format dicionary (formatDictionary) that was passed in.
# Parameters:
#  I    atString				String that holds a format and its value separated by and "="
#  I    formatDictionary		Dictionary that holds every format : value
# Returns:
#       None
# Notes:
#		Started off the same as setVar but more complicated due to multiple cases
#       Working with lists made it a lot easier to parse and compare with cases
#       For the JUST tag, the values must be LEFT, RIGHT, BULLET or CENTER!!!
#       I also hardcoded the given tags so I can check if a tag existed in the
#           tag list in the first place I hope that is fine.
#       The value of BULLET is meaningless when JUST isn't "BULLET".
###############################################################################################
def setFormat(atString, formatDictionary):
    #pattern to check atString format
    #group 1 = tag key
    #group 2 = value of tag could be # or Letter so I used \S
    ercheck = re.compile(r'(\w*)=(\S*)')
    if ercheck.match(atString):
        errcheck = True
    else:
        print("*** Invalid format, found: ", atString.rstrip())
        return None

    #List of valid tag keys:  LM, RM, JUST, BULLET, FLOW
    tagList = ["LM", "RM", "JUST", "BULLET", "FLOW"]

    # Splitting on whitespace(s), and = sign
    match = re.split("[\s=]\s*", atString)

    # Creating a list of the formats
    # Slice notation a[start:stop:step]
    # step 2 to match tags from start
    formats = match[::2]
    if formats[-1] == "":
        formats.pop()
    # Creating a list of the values
    # Step 2 when offset 1 from start to match
    # Values
    values = match[1::2]

    #Making a list of format values to clean up below code
    ###For the JUST tag, the values must be LEFT, RIGHT, BULLET or CENTER
    justValues = ["LEFT", "RIGHT", "BULLET", "CENTER"]
    # For the flow values im assuming the only acceptable values are specifically
    # the words yes or no
    flowValues = ["YES","NO"]

    # Iterate through the keys in dictionary
    for key in formatDictionary:
        # Check if key = format[i]
        for i in range(0, len(formats)):
            # Checks if formats is FLOW and matches key.
            if key == formats[i] and key == "FLOW":
                if values[i] in flowValues:
                    formatDictionary[key] = values[i]
                else:
                    print("*** Bad value for", formats[i], "found:", values[i])
            # Going to start with JUST as it is has multiple cases
            # Checks if current key is JUST
            elif key == formats[i] and key == "JUST":
                ###For the JUST tag, the values must be LEFT, RIGHT, BULLET or CENTER
                if values[i] in justValues:
                    formatDictionary[key] = values[i]
                else:
                    print("*** Bad value for", formats[i], "found:", values[i])
            # Special BULLET Case
            # Note that the value of BULLET is meaningless when JUST isn't "BULLET".
            elif key == formats[i] and key == "BULLET":
                #if formats[i-1] != "JUST" and values[i-1] != "BULLET":
                #    print("BULLET value given when JUST != BULLET")
                if values[i] == "o":
                    formatDictionary[key] = values[i]
                else:
                    print("*** Bad value for", formats[i], "found:", values[i])
            # Checks if formats is RM and matches key.
            elif key == formats[i] and key == "RM":
                #default is 80
                if values[i].isdigit():
                    formatDictionary[key] = values[i]
                else:
                    print("*** Bad value for", formats[i], "found:", values[i])
                    # Checks if formats is LM and matches key.
            elif key == formats[i] and key == "LM":
                #default is 5
                if values[i].isdigit():
                    formatDictionary[key] = values[i]
                else:
                    print("*** Bad value for", formats[i], "found:", values[i])
    # If none of the above check if the tag exists at all
    if formats[i] not in tagList:
        print("*** Invalid format, found:", formats[i],"=",values[i])
