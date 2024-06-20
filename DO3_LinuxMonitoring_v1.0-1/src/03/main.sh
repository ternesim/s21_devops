#!/bin/bash

if [[ "$#" != 4 ]]; then 
  echo "wrong arg number";
  exit;
fi
for i in $@
do
  SEG=$(echo $i | sed -n '/^[1-6]$/p');
  if [[ -z "$SEG" ]]; then 
    echo "bad args";
  exit;
fi
done
if [[ $1 == $2 ]] || [[ $3 == $4 ]]; then 
  echo "matched text and background colors";
  exit;
fi

HOSTNAME=$(hostname)

TIMEZONE=$(timedatectl | sed -n 's/[ ]*Time zone://p' | sed 's/(.*)//')
TIMEZONE+="UTC $(date "+%-Z" | sed "s/0*//g; s/\.0*/./g")"

USER=$(whoami)
OS=$(cat /etc/issue | sed 's/\\n \\l//')
DATE=$(date '+%d %b %Y %H:%M:%S')
UPTIME=$(uptime | awk '{print $3, $4}' | tr , " ")
UPTIME_SEC=$(awk '{print $1}' /proc/uptime)
IP=$(hostname -i | awk '{print $1}')

INTERFACE=$(ip -o -f inet route |grep -e "^default" |awk '{print $5}')
IPC=$(ip -o -f inet addr show | grep "$INTERFACE" | awk '/scope global/ {print $4}')
MASK=$(ipcalc $IPC | awk 'FNR == 2 {print $2}')

GATEWAY=$(ip r | grep '^def' | awk '{print $3}')

TEMP=$(free --mega | awk 'FNR == 2 {print $2}')
RAM_TOTAL=$(echo "scale=2;$TEMP/1024" | bc)

TEMP=$(free --mega | awk 'FNR == 2 {print $3}')
RAM_USED=$(echo "scale=2;$TEMP/1024" | bc)

TEMP=$(free --mega | awk 'FNR == 2 {print $4}')
RAM_FREE=$(echo "scale=2;$TEMP/1024" | bc)

SPACE=$(df -k . | awk 'FNR == 2 {print $2}')
SPACE_ROOT=$(echo "scale=2;$SPACE/1024" | bc)

USED_SPACE=$(df -k . | awk 'FNR == 2 {print $3}')
SPACE_ROOT_USED=$(echo "scale=2;$USED_SPACE/1024" | bc)

FREE=$(df -k . | awk 'FNR == 2 {print $4}')
SPACE_ROOT_FREE=$(echo "scale=2;$FREE/1024" | bc)

. output.sh