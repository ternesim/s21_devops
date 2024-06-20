all: s21_grep

s21_grep: s21_grep.o
	gcc -Wall -Wextra -Werror -std=c11 s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c
	gcc -Wall -Wextra -Werror -std=c11 -c s21_grep.c

test:
	gcc s21_grep.c -g -Wall -Wextra -Werror -std=c11 -o s21_grep
	./test_func_grep.sh > test_log.txt
	./valgrind_two_flags.sh > test_leak_log.txt
	
clean: 
	rm -rf *.o  .

rebuild: clean s21_grep