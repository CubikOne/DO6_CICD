FLAGS = -Wall -Wextra -Werror

all: s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	gcc $(FLAGS) s21_grep.c -o s21_grep

rebuild:
	make clean
	make all

clean:
	rm -f s21_grep