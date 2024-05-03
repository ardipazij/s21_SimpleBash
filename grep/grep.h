#ifndef GREP_H
#define GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 4096
typedef struct Flags {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

void init_flags(Flags *flags);
int parser(int argc, char *argv[], Flags *flag, char *find);
void detected_flags(Flags Flags, char *find);
int parser_files_for_f_flag(char *find, char *filenames);
void error_exception(int res);
void parser_files(int argc, char *argv[], Flags *flag, int optind,
                  char *pattern);
#endif

