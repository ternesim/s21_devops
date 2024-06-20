 #!/bin/bash

cd src/$1 &&\
bash test_func_$1.sh &&\
bash ../telegram_bot.sh "$1 integration test passed"