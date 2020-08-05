#!/bin/bash


read -n3 -p "Enter the apartment number : " aptNum
read -p "Enter the payment amount : " payment

if [ -e data/$aptNum.apt ]; then
  dos2unix readcont.bash
  bash readcont.bash < data/$aptNum.apt
  balance = balance - payment
  echo "$balance"
else
  echo -e "ERROR: apartment not found"
fi


echo "[date] PAID: $aptNum - AMOUNT: $payment - NEW BALANCE: $balance" >> data/queries.log