#!/bin/bash

if [[ "$#" != 1 ]]; then 
  echo "wrong arg number";
  exit;
fi
RES=$(echo $1 | sed -n '/[0-9]/p');
if [[ ! -z "$RES" ]]; then 
  echo "arg is number";
else 
  echo "$1"
fi