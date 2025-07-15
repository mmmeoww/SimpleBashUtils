CC=gcc
CFLAGS=-c -Wall -Werror -Wextra -std=c11

s21_cat : s21_cat.o 
	$(CC) s21_cat.o -o s21_cat

cat.o : s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) s21_cat.c

check :
	clang-format -n *.c
	clang-format -n *.h
clean :
	rm -rf  *.o *.out
	rm -rf  s21_cat
