# Create a class that we can inspect using Reflection
class Stuff:
  # Constructor
  def __init__(self):
    # Give the instance some ints, floats and strings
    self.a = 1
    self.b = 200.0
    self.c = 3
    self.d = 400.0
    self.m = "a"
    self.n = "xy"
  
  # Print all the values on one line
  def prt(self):
    # Creates and prints a list
    print([self.a, self.b, self.c, self.d, self.m, self.n])
    
# Function to display field names and types
def info(obj):
  for nam,val in vars(obj).items():
    print("** Field = " + nam + " is " + type(val).__name__)

# Function using Reflection to modify values in a class
def modify(obj):
  # Iterate through all the fields in a class (not the methods)
  for nam,val in vars(obj).items():
    # If the field is an integer, multiply the value by 10
    if (isinstance(val, int)):
      setattr(obj, nam, 10*val)
    elif (isinstance(val, float)):
      setattr(obj, nam, val + 10.0)
    elif (isinstance(val, str)):
      setattr(obj, nam, val+val)
    
# Create an instance of the class and print it
s = Stuff(); info(s); s.prt()

# Modify it and print it a couple times
modify(s); s.prt()
modify(s); s.prt()
