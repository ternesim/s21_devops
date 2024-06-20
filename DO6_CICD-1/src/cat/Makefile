all: s21_cat

s21_cat: s21_cat.o
	gcc -Wall -Wextra -Werror -std=c11 s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c
	gcc -Wall -Wextra -Werror -std=c11 -c s21_cat.c

test:
	gcc s21_cat.c -g -Wall -Wextra -Werror -std=c11 -o s21_cat
	./test_func_cat.sh > test_log.txt
	./valgrind_two_flags.sh > test_leak_log.txt
	
clean: 
	rm -rf *.o  .

rebuild: clean s21_cat