CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11

all: s21_grep

s21_grep : s21_grep.o 
	$(CC) s21_grep.o -o s21_grep

grep.o : s21_grep.c s21_grep.h
	$(CC) $(CFLAGS) s21_grep.c

check :
	clang-format -n *.c
	clang-format -n *.h

clean :
	rm -rf *.o *.out
	rm -rf s21_grep