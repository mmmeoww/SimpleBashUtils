#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int e, i, v, c, l, n, h, s, f, o, pat_size;
  char *pat;
} ops;

void parser(int argc, char *argv[], ops *options);
void grep(ops *options, int argc, char *argv[]);
void output_str(char *str, int n);
void file_reader(ops *options, char *filepath, regex_t *reg);
int logic(int exec, ops *options, int *str_count, int *match_count,
          char *filepath);
void o_flag(ops *options, regex_t *reg, char *str, char *filepath,
            int *str_count);
void many_pat(ops *options, char *new_pat);
void reg_file(char *filepath, ops *options);
int s21_getline(char **lineptr, size_t *n, FILE *stream);
void add_char(ops *options, char sym);

#endif