#include "s21_cat.h"

int main(int argc, char *argv[]) {
  ops options = {0};
  int str_count = 0;
  int new_line = 1;
  int last = 1;
  parser(argc, argv, &options);
  if (!options.err) {
    for (int i = optind; i < argc; i++) {
      reading(argv, &options, i, &str_count, &new_line, &last);
    }
  }
  return 0;
}
void reading(char **argv, ops *options, int file_arg, int *str_count,
             int *new_line, int *last) {
  FILE *filepath = fopen(argv[file_arg], "r");
  if (filepath == NULL) {
    perror("s21_cat");
  } else {
    int continue_flag = 0;
    int current = 0, empty_count = 0, prev = '\n';
    while ((current = fgetc(filepath)) != EOF) {
      if (options->s && current == '\n' && prev == '\n') {
        empty_count++;
        if (empty_count > 1) {
          continue_flag = 1;
        }
      } else {
        empty_count = 0;
        continue_flag = 0;
      }

      if (!continue_flag) {
        if (options->n) n_flag(current, str_count, last);
        if (options->b) b_flag(current, str_count, new_line);
        if (options->e) e_flag(current);
        if (options->t) t_flag(&current);
        if (options->v) v_flag(&current);

        fputc(current, stdout);
      }
      prev = current;
    }
    fclose(filepath);
  }
}
void parser(int argc, char **argv, ops *options) {
  int curr_op = 0;
  const struct option longOpions[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'n'},
                                      {"squeeze-blank", 0, NULL, 's'},
                                      {NULL, 0, NULL, 0}};
  while ((curr_op = getopt_long(argc, argv, "+benstvET", longOpions, NULL)) !=
         -1) {
    switch (curr_op) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      default:
        options->err = 1;
        break;
    }
  }
  if (options->b) options->n = 0;
}

void n_flag(int current, int *str_count, int *last) {
  if (*last) {
    printf("%6d\t", ++(*str_count));
    *last = 0;
  }
  if (current == '\n') {
    *last = 1;
  }
}

void b_flag(int current, int *str_count, int *new_line) {
  if (*new_line && current != '\n') {
    printf("%6d\t", ++(*str_count));
    *new_line = 0;
  }
  if (current == '\n') {
    *new_line = 1;
  }
}

void e_flag(int current) {
  if (current == '\n') {
    printf("$");
  }
}

void t_flag(int *current) {
  if (*current == '\t') {
    printf("^");
    *current = 'I';
  }
}

void v_flag(int *current) {
  if (*current != 9 && *current != 10) {
    if (*current == 0) {
      printf("^");
      *current = '@';
    } else if (*current >= 1 && *current <= 31) {
      printf("^");
      *current += 64;
    } else if (*current == 127) {
      printf("^");
      *current = '?';
    } else if (*current >= 128) {
      printf("M-");
      *current -= 128;
    }
  }
}
