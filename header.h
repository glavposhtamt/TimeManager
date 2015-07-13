#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 128

struct myTymeInfo {
  struct tm arrTimeinfo[128];
  unsigned short length;
};

typedef struct myTymeInfo mt;

extern char ** readWriteFile();
extern  mt * getStruct( char ** bigstr );
extern int a;

#endif // HEADER

