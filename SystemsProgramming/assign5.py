#!/usr/bin/env python3
import os
import sys
import glob
import errno

#example input
#assign5.py ./data assign5.template 12/16/2021 ./output ’{’ ’|’

if not os.path.isdir(sys.argv[1]):
    print(sys.argv[1] + " is not a directory")
    sys.exit(1)

path =sys.argv[1]
files=glob.glob(path)
#uh
for name in files:
	try:
		with open(name) as f:
			sys.stdout.write(f.read())
			
	except IOError as exc:
		if exc.errno != errno.EISDIR:
			raise