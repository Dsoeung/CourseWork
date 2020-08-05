#assign2

/^.*..DL [0-9]/ s/[0-9]/X/g
s/\(^.*..DL\) X*/\1 XXXXXX/g

#Master card begin with 5, have 16 digits, 5XXX-XXXX-XXXX-XXXX, 4-4-4-4 MC
s/\(^.*\)5[0-9]\+-[0-9]\+-[0-9]\+-\([0-9]\+\)/\1MC - \2/g

#Visa card begin with 4, have 16 digits, 4XXXXXXXXXXXXXXX, 16 VISA
s/\(^.*\)4[0-9]\{11\}\([0-9]\+\)/\1Visa-\2/g

#America Express begin with 3 +[47], have 16 digits, 3[47]XX-XXXXXX-XXXXX, 4-6-5 AMEC
s/\(^.*\)3[47][0-9]\+-[0-9]\+-.\([0-9]\+\)/\1AMEC-\2/g

#Discover Cards begin with 6, have 16 digits, 6XXX-XXXX-XXXX-XXXX, 4-4-4-4 DISC
s/\(^.*\)6[0-9]\+-[0-9]\+-[0-9]\+-\([0-9]\+\)/\1Disc-\2/g

#License Plates must match first cause they're in the middle of initial paragraph, use . to option select between hyphens and spaces
/TX.[A-Z]\{2\}..[0-9]\{3\}.*/ s/TX.[A-Z]\{2\}..[0-9]\{3\}./TX XXXXXX/g
/TX.[A-Z0-9]\{2\}..[A-Z0-9]\{2\}.*/ s/TX.[A-Z0-9]\{2\}..[A-Z0-9]\{2\}./TX XXXXXX/g

#date
s:<date>:${date}:

#city
s/<orgname>/City of Gainsville, Florida/g






