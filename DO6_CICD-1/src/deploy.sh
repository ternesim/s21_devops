 #!/bin/bash

#if "Host key verification failed" error:
#sudo su gitlab-runner
#ssh-copy-id -i ~/.ssh/id_rsa.pub ternesim@192.168.31.64

cd src/$1 &&\
scp -r build/s21_$1 ternesim@192.168.31.64:/home/ternesim/ &&\
ssh ternesim@192.168.31.64 "echo 0856 | sudo -S mv /home/ternesim/s21_$1  /usr/local/bin/ " &&\
bash ../telegram_bot.sh "$1 deployed"