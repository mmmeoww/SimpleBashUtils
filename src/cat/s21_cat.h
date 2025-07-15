#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>

typedef struct options {
  int b, e, n, s, t, v, err;
} ops;

void reading(char **argv, ops *options, int file_arg, int *str_count,
             int *new_line, int *last);
void parser(int argc, char **argv, ops *options);
void n_flag(int current, int *str_count, int *last);
void b_flag(int current, int *str_count, int *new_line);
void e_flag(int current);
void t_flag(int *current);
void v_flag(int *current);

#endif