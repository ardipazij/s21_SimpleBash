#ifndef CAT_H
#define CAT_H
#include <stdio.h>
#include <stdlib.h>

#include "string.h"
typedef struct Flags {
  int b;
  int n;
  int s;
  int E;
  int e;
  int t;
  int T;
  int v;
} Flags;

void init_flag(Flags* flag);
void default_cat(FILE* file);
int parser_flags(Flags* flag, int argc, char** argv);
void parser_files(int position, int argc, char** argv, Flags flag);

#endif

