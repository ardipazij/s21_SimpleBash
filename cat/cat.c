#include "cat.h"

int main(int argc, char* argv[]) {
  Flags flags;
  init_flag(&flags);
  int position = parser_flags(&flags, argc, argv);
  parser_files(position, argc, argv, flags);
  return 0;
}

int parser_flags(Flags* flags, int argc, char** argv) {
  int i = 0, flag = 1;
  for (i = 1; i < argc && argv[i][0] == '-'; i++) {
    if (strlen(argv[i]) > 1 && argv[i][1] == '-' && flag) {
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        flags->b = 1;
      } else if (strcmp(argv[i], "--number") == 0) {
        flags->n = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        flags->s = 1;
      } else {
        fprintf(
            stderr,
            "\ncat: illegal option -- %s\nusage: cat [-benstuv] [file ...]\n",
            argv[i]);
        flag = 0;
      }
    } else {
      for (size_t j = 1; j < strlen(argv[i]) && flag; j++) {
        if (argv[i][j] == 'b') {
          flags->b = 1;
        } else if (argv[i][j] == 's') {
          flags->s = 1;
        } else if (argv[i][j] == 'n') {
          flags->n = 1;
        } else if (argv[i][j] == 't') {
          flags->T = 1;
          flags->v = 1;
        } else if (argv[i][j] == 'e') {
          flags->E = 1;
          flags->v = 1;
        } else if (argv[i][j] == 'E') {
          flags->E = 1;
        } else if (argv[i][j] == 'T') {
          flags->T = 1;
        } else if (argv[i][j] == 'v') {
          flags->v = 1;
        } else {
          fprintf(
              stderr,
              "\ncat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
              argv[i][j]);
          flag = 0;
        }
      }
    }
  }
  i = (!flag) ? flag : i;
  return i;
}

void init_flag(Flags* flag) {
  flag->b = 0;
  flag->E = 0;
  flag->n = 0;
  flag->s = 0;
  flag->T = 0;
  flag->v = 0;
}

void parser_files(int position, int argc, char** argv, Flags flags) {
  for (int i = position; i < argc; i++) {
    FILE* file = fopen(argv[i], "r");
    if (file == NULL) {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
      return;
    }
    int number = 1, temp = '\n', empty_string = 0, symbol;
    while ((symbol = getc(file)) != EOF) {
      int flag = 1;
      if (flags.s && symbol == '\n' && temp != '\n') {
        empty_string++;
      } else if (flags.s && temp == '\n' && symbol == '\n') {
        empty_string++;
      } else if (flags.s && temp == '\n' && symbol != '\n') {
        empty_string = 0;
      }
      if (flags.n && !flags.b && temp == '\n' && empty_string < 3) {
        printf("%6d\t", number++);
      } else if (flags.b && temp == '\n' && symbol != '\n') {
        printf("%6d\t", number++);
      }
      if (empty_string < 3 && flags.T && symbol == 9) {
        printf("^I");
        flag--;
      }
      if (empty_string < 3 && flags.E && symbol == 10) {
        printf("$\n");
        flag--;
      }
      if (flags.v && symbol == 127) {
        printf("^?");
        flag--;
      } else if (flags.v && symbol < 32 && symbol != 10 && symbol != 9) {
        printf("^%c", symbol + 64);
        flag--;
      }
      if ((!feof(file)) && empty_string < 3 && flag) {
        putchar(symbol);
      }
      temp = symbol;
    }
    fclose(file);
  }
}
