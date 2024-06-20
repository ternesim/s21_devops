 #!/bin/bash

cd src/$1/ &&\
make &&\
mkdir -p build &&\
mv s21_$1 build &&\
bash ../telegram_bot.sh "$1 has been built"