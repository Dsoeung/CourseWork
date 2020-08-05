from collections import OrderedDict

#initialize ordered dict. We're checking in reverse order starting from the largest values to build the roman numeral left to right.
def numToRom():  
    od = OrderedDict()
    od[3000] = "MMM"
    od[2000] = "MM"
    od[1000] = "M"
    od[900] = "CM"
    od[800] = "DCCC"
    od[700] = "DCC"
    od[600] = "DC"
    od[500] = "D"
    od[400] = "CD"
    od[300] = "CCC"
    od[200] = "CC"
    od[100] = "C"
    od[90] = "XC"
    od[80] = "LXXX"
    od[70] = "LXX"
    od[60] = "LX"
    od[50] = "L"
    od[40] = "XL"
    od[30] = "XXX"
    od[20] = "XX"
    od[10] = "X"
    od[9] = "IX"
    od[8] = "VIII"
    od[7] = "VII"
    od[6] = "VI"
    od[5] = "V"
    od[4] = "IV"
    od[3] = "III"
    od[2] = "II"
    od[1] = "I"
    return od

#Initialize ordered dict for rom to num conversion, same idea takin highest placed value and building the number.
def romToNum():
    od = OrderedDict()
    od["MMM"] = 3000
    od["MM"] = 2000
    od["M"] = 1000
    od["CM"] = 900
    od["DCCC"] = 800
    od["DCC"] =700
    od["DC"] = 600
    od["D"] = 500
    od["CD"] = 400
    od["CCC"] = 300
    od["CC"] = 200
    od["C"] = 100
    od["XC"] = 90
    od["LXXX"] = 80
    od["LXX"] = 70
    od["LX"] = 60
    od["L"] = 50
    od["XL"] = 40
    od["XXX"] = 30
    od["XX"] = 20
    od["X"] = 10
    od["IX"] = 9
    od["VIII"] = 8
    od["VII"] = 7
    od["VI"] = 6
    od["V"] = 5
    od["IV"] = 4
    od["III"] = 3
    od["II"] = 2
    od["I"] = 1
    return od
#ErrCheck based off single values, [M][D][C][L][I][V][X]

ErrCheck = ["MMMM", "DD", "CCCC", "LL", "XXXX", "VV", "IIII"]
#init number = roman list
RomanList = numToRom() 
#init roman = number list
NumerList = romToNum()


# FUNCTION 1 INT TO ROMAN NUMERAL
def convertToRoman(num):
    #basic error check for valid number range
    if num > 3999:
        raise ValueError("Error - too large")
        return super().__init__(cls, num)
    if num <= 0:
        raise ValueError("Error - Value must be greater than 0")
        return super().__init__(cls, num)
        
    #initialize answer, building the roman numeral
    answer = []
    for key in RomanList:
        if key > num: continue
        #check if num can be clean divided, starts from largest
        quot = num // key
        if not quot: continue
        #if quot = 1, add roman value equivalent
        answer.append( RomanList[key] * quot )
        num -= (key * quot)
        if not num: break
    return "".join(answer)
    


# FUNCTION 2 ROMAN NUMERAL TO INT
def convertToNum(num):
    #Convert to entry to uppercase, cause dict is in all upper
    rom = num.upper()
    #Error check on input
    if any(sub in num for sub in ErrCheck):
        raise ValueError("ERROR: Invalid Roman Numeral Entered")
        return super().__init__(cls, num)
    if ' ' in num:
        #error if any white space in input, hope this is what it asked for.
        raise ValueError("ERROR: White Space Detected")
        return super().__init__(cls, num)
    #initialize answer
    answer = 0
    #Enumerate through input
    for i,c in enumerate(rom):
        #if near end or if key matches add else subtract
        if (i+1) == len(rom) or NumerList[c] >= NumerList[rom[i+1]]:
            answer += NumerList[c]
        else:
        #subtract from answer
            answer -= NumerList[c]
    return answer



#Everything below is to test values and functions
#val = int(input("Enter your value: ")) 
#print(int(val))
#num = 35
#print (convertToRoman(val))
#sr = input("Enter roman numeral : ")
#print (convertToNum(sr))