 #!/bin/bash

cd src/$1 &&\
clang-format --dry-run --Werror s21_$1.c &&\
bash ../telegram_bot.sh "$1 codestyle passed"