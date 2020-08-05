#!/bin/bash
for i in {1..7}; do
  echo ========= mmm$i
  gcc -Wall mmm$i.c -o ~/ProgL/HW3/tmp/mmm$i
  ~/ProgL/HW3/tmp/mmm$i
  ~/ProgL/HW3/tmp/mmm$i | md5sum
done