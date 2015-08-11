#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 128

struct myTymeInfo {
  struct tm arrTimeinfo;
  char * message;
  unsigned short length;
};

typedef struct myTymeInfo mt;

extern char ** readFile();
extern void writeFile();
extern  mt * getStruct( char ** bigstr );
extern int a;

#include "function.c"

#endif // HEADER

