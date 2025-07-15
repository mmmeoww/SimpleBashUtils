#include "s21_grep.h"

int main(int argc, char *argv[]) {
  ops options = {0};
  parser(argc, argv, &options);
  grep(&options, argc, argv);
  free(options.pat);
  return 0;
}

void parser(int argc, char *argv[], ops *options) {
  int curr_op = 0;
  while ((curr_op = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (curr_op) {
      case 'e':
        options->e = 1;
        many_pat(options, optarg);
        break;
      case 'i':
        options->i = REG_ICASE;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        reg_file(optarg, options);
        break;
      case 'o':
        options->o = 1;
        break;
      default:
        perror("./s21_grep");
        break;
    }
  }
  if (options->pat == NULL) {
    if (optind < argc) {
      many_pat(options, argv[optind]);
      optind++;
    } else {
      perror("./s21_grep");
    }
  }
  add_char(options, '\0');
  if (argc - optind == 1) {
    options->h = 1;
  }
}

void many_pat(ops *options, char *new_pat) {
  if (options->pat_size > 0) {
    add_char(options, '|');
    add_char(options, '(');
  } else {
    add_char(options, '(');
  }

  int len = (int)strlen(new_pat);
  for (int i = 0; i < len; i++) {
    add_char(options, new_pat[i]);
  }

  add_char(options, ')');
}

void add_char(ops *options, char sym) {
  options->pat_size += 1;
  char *temp = realloc(options->pat, sizeof(char) * options->pat_size);
  if (temp == NULL) {
    perror("./s21_grep");
    free(temp);
  } else {
    options->pat = temp;
    options->pat[options->pat_size - 1] = sym;
  }
}

void reg_file(char *filepath, ops *options) {
  FILE *reg_file = fopen(filepath, "r");
  if (reg_file == NULL) {
    if (!options->s) {
      perror("./s21_grep");
    }
  } else {
    char *str = NULL;
    size_t mem = 0;
    int n = 0;

    while ((n = s21_getline(&str, &mem, reg_file)) != -1) {
      if (str[n - 1] == '\n') {
        str[n - 1] = '\0';
      }
      many_pat(options, str);
    }
    free(str);
    fclose(reg_file);
  }
}

void file_reader(ops *options, char *filepath, regex_t *reg) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    if (!options->s) {
      perror("./s21_grep");
    }
  } else {
    char *str = NULL;
    size_t mem = 0;
    int str_count = 1;
    int match_count = 0;
    int n = 0;
    int exec = 1;
    while ((n = s21_getline(&str, &mem, file)) != -1) {
      exec = regexec(reg, str, 0, NULL, 0);
      if (logic(exec, options, &str_count, &match_count, filepath)) {
        output_str(str, n);
      }
      if (options->o && !options->l && !options->c) {
        o_flag(options, reg, str, filepath, &str_count);
      }
    }

    if (options->l && match_count >= 1) {
      printf("%s\n", filepath);
    }
    if (options->c && !options->l) {
      if (!options->h) {
        printf("%s:", filepath);
      }
      printf("%d\n", match_count);
    }
    free(str);
    fclose(file);
  }
}

void output_str(char *str, int n) {
  for (int i = 0; i < n; i++) {
    fputc(str[i], stdout);
  }
  if (str[n - 1] != '\n') fputc('\n', stdout);
}

int logic(int exec, ops *options, int *str_count, int *match_count,
          char *filepath) {
  int out = 0;
  if ((!exec && !options->v) || (exec && options->v)) {
    (*match_count)++;
  }
  if (((!exec && !options->v) || (exec && options->v)) && !options->c &&
      !options->l && !options->o) {
    if (!options->h) printf("%s:", filepath);
    if (options->n) printf("%d:", *str_count);
    out = 1;
  }
  (*str_count)++;
  return out;
}
void grep(ops *options, int argc, char *argv[]) {
  regex_t regex;
  if (regcomp(&regex, options->pat, REG_EXTENDED | options->i)) {
    perror("./s21_grep");
  }
  for (int i = optind; i < argc; i++) {
    file_reader(options, argv[i], &regex);
  }
  regfree(&regex);
}

void o_flag(ops *options, regex_t *reg, char *str, char *filepath,
            int *str_count) {
  regmatch_t match;
  int shift = 0;
  while (1) {
    int res = regexec(reg, str + shift, 1, &match, 0);
    if (res != 0) {
      break;
    }
    if (!options->c && !options->v) {
      if (!options->h) printf("%s:", filepath);
      if (options->n) {
        printf("%d:", (*str_count) - 1);
      }

      for (int i = (int)match.rm_so; i < (int)match.rm_eo; i++) {
        fputc(str[shift + i], stdout);
      }
      fputc('\n', stdout);
    }
    shift += (int)match.rm_eo;
  }
}
int s21_getline(char **lineptr, size_t *n, FILE *filestream) {
  int flag = 0;
  if (*lineptr == NULL || *n == 0) {
    *n = 10;
    *lineptr = (char *)malloc(*n * sizeof(char));
    if (*lineptr == NULL) {
      free(lineptr);
      flag = 1;
    }
  }
  char *buffer = *lineptr;
  size_t capacity = *n;
  int length = 0, c = 0, done = 0;
  while (!done && flag != 1) {
    c = fgetc(filestream);
    if (c == EOF) {
      if (length == 0) {
        flag = 1;
      }
      done = 1;
    } else {
      if (length + 1 >= (int)capacity) {
        capacity *= 2;
        char *new_buffer = (char *)realloc(buffer, capacity);
        if (new_buffer == NULL) {
          free(new_buffer);
          flag = 1;
          done = 1;
        } else {
          buffer = new_buffer;
          *lineptr = buffer;
          *n = capacity;
        }
      }
      if (flag != 1) {
        buffer[length++] = (char)c;
        if (c == '\n') {
          done = 1;
        }
      }
    }
  }
  buffer[length] = '\0';
  int ret = flag == 0 ? length : -1;
  return ret;
}
