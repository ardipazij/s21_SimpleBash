#include "grep.h"

int main(int argc, char *argv[]) {
  char found_patterns[4096] = {'\0'};
  Flags flag;
  init_flags(&flag);
  if (parser(argc, argv, &flag, found_patterns))
    parser_files(argc, argv, &flag, optind, found_patterns);
  return 0;
}

void init_flags(Flags *flags) {
  flags->e = 0;
  flags->i = 0;
  flags->v = 0;
  flags->c = 0;
  flags->l = 0;
  flags->n = 0;
  flags->h = 0;
  flags->s = 0;
  flags->f = 0;
  flags->o = 0;
}

int parser(int argc, char *argv[], Flags *flag, char *found_patterns) {
  const char *options = "e:ivclnhsf:o";
  int res = 0;
  int counter = 0;
  int flag_error_read_file = 1;
  while ((res = getopt_long(argc, argv, options, NULL, NULL)) != -1 &&
         flag_error_read_file) {
    if (res == 'e') {
      if (counter++) strcat(found_patterns, "|");
      flag->e = 1;
      strcat(found_patterns, optarg);
    } else if (res == 'i') {
      flag->i = 1;
    } else if (res == 'v') {
      flag->v = 1;
    } else if (res == 'c') {
      flag->c = 1;
    } else if (res == 'l') {
      flag->l = 1;
    } else if (res == 'n') {
      flag->n = 1;
    } else if (res == 'h') {
      flag->h = 1;
    } else if (res == 's') {
      flag->s = 1;
    } else if (res == 'f') {
      flag->f = 1;
      if (parser_files_for_f_flag(found_patterns, optarg)) {
        flag_error_read_file = 0;
      }
    } else if (res == 'o') {
      flag->o = 1;
    } else {
      error_exception(res);
    }
  }
  return flag_error_read_file;
}

// void detected_flags(Flags flags, char * found_patterns){
//     printf("%s\n", found_patterns);
//     if (flags.e) {
//       printf("flag e detected\n");
//     }
//     if (flags.i) {
//       printf("flag i detected\n");
//     }
//     if (flags.v) {
//         printf("flag v detected\n");
//     }
//     if (flags.c) {
//       printf("flag c detected\n");
//     }
//     if (flags.l) {
//       printf("flag l detected\n");
//     }
//     if (flags.n) {
//       printf("flag n detected\n");
//     }
//     if (flags.h) {
//       printf("flag h detected\n");
//     }
//     if (flags.s) {
//       printf("flag s detected\n");
//     }
//     if (flags.f) {
//       printf("flag f detected\n");
//     }
//     if (flags.o) {
//       printf("flag o detected\n");
// }
// }

int parser_files_for_f_flag(char *found_patterns, char *filenames) {
  FILE *file;
  int open_error = 0;
  file = fopen(filenames, "r");
  if (file == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory\n", filenames);
    open_error = 1;
  } else {
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      int count_patterns = strlen(found_patterns);
      if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
      }
      if (count_patterns) {
        strcat(found_patterns, "|");
      }
      strcat(found_patterns, buffer);
    }
    fclose(file);
  }
  return open_error;
}

void error_exception(int res) { printf("opana moment %c\n\n\n\n\n", res); }

void parser_files(int argc, char *argv[], Flags *flag, int optind,
                  char *pattern) {
  regex_t reg;
  if (!flag->e && !flag->f) pattern = argv[optind++];
  int flag_many_files = (argc - optind > 1), compilation = REG_EXTENDED;
  if ((argc - optind) > 0) {
    for (int k = optind; k < argc; k++) {
      char *filename = argv[k];
      FILE *file = fopen(filename, "r");
      if (file) {
        if (flag->i == 1) compilation = REG_ICASE;
        if (regcomp(&reg, pattern, compilation) == 0) {
          char str[4096];
          regmatch_t pmatch[1];
          int line = 0, number = 1, mc = 0;
          while (fgets(str, 4095, file) != NULL) {
            int match = 0, res = regexec(&reg, str, 1, pmatch, 0);
            if (strchr(str, '\n') == NULL) strcat(str, "\n");
            if (res == 0 && flag->v != 1) match = 1, mc = mc + 1;
            if (res == REG_NOMATCH && flag->v == 1) match = 1, mc = mc + 1;
            if ((match == 1 && flag->h == 1 && flag->l == 1 && flag->c == 1 &&
                 !flag_many_files))
              printf("%s:", filename);
            if (match == 1 && flag->h != 1 && flag->l != 1 && flag_many_files)
              printf("%s:", filename);
            if (match == 1 && flag->n == 1 && flag->c != 1 && flag->l != 1)
              printf("%d:", number);
            if (match == 1 && flag->o != 1 && flag->c != 1 && flag->l != 1)
              printf("%s", str);
            if (match == 1 && flag->o == 1) {
              for (int i = (int)pmatch[0].rm_so; i < pmatch[0].rm_eo; i++)
                printf("%c", str[i]);
              printf("\n");
            }
            line = line + match, number = number + 1;
          }
          if (flag->c == 1 && flag->l == 1) printf("%s:", filename);
          if (flag->l == 1 && line > 1) line = 1;
          if (flag->c == 1) printf("%d\n", line);
          if (flag->l && mc) printf("%s\n", filename);
          regfree(&reg);
        } else {
          fprintf(stderr, "grep: rega huega\n");
        }
        fclose(file);
      } else {
        if (flag->s != 1) fprintf(stderr, "fail open %s\n", filename);
      }
    }
  }
}

