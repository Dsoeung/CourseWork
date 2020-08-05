#!/bin/bash


sed -i '

#Replace ssn with %%%-%%-%%%%
s/[0-9]{3}-[0-9]{2}-[0-9]{4}/$$$-$$-$$$$/g

#Replace phone number
#111-111-1111 format

s/[0-9]{3}-[0-9]{3}-[0-9]{4}/###-###-####/g

#111-1111 format
s/[0-9]{3}-[0-9]{4}/###-####/g

#(111)-111-1111 format
s/\([0-9]{3}\)-[0-9]{3}-[0-9]{4}/\(###\)-###-####/g

# strings between %%
s/%.*%//g

# \* replaced with \* ATTENTION \* 
/^\*[ ]+/i\\n* ATTENTION *\n

#date from DD/MM/YYYY to YYYY-MM-DD
s/([0-9]+)\/([0-9]{2})\/([0-9]{4})/\3-\2-\1/g

' "$@"