#!/bin/bash
# this reads from stdin

echo "Enter one of the following actions or press CTRL-D to exit.\m"
echo "C - create a new course record"
echo "R - read an existing course record"
echo "U - update an existing course record"
echo "D - delete an existing course record"
echo "E - update enrolled student count of existed course"
echo "T - show total course count"

#n1 so that they can only enter 1 character
while true; do
read -n3 -p "Enter the action (CTRL-C to stop) :" action
#dependent on action to choose which FUNCTION to run

if [[ -z "$action" ]] 
then
  echo "No action given try again"
fi

if [ "$action" = c ] || [ "$action" = C ] 
then 
  dos2unix create.bash
  bash create.bash
fi 

if [ "$action" = r ] || [ "$action" = R ] 
then 
  dos2unix read.bash
  bash read.bash
fi 

if [ "$action" = u ] || [ "$action" = U ] 
then 
  dos2unix update.bash
  bash update.bash
fi 

if [ "$action" = d ] || [ "$action" = D ] 
then 
  dos2unix delete.bash
  bash delete.bash
fi 

if [ "$action" = e ] || [ "$action" = E ] 
then 
  dos2unix enroll.bash
  bash enroll.bash
fi 

if [ "$action" = t ] || [ "$action" = T ] 
then 
  dos2unix total.bash
  bash total.bash
fi 

done
