#include "function.c"

#define SIZE 128

int main(){
    int i = 0;
    char ** bigstr;
    mt * myt;
    bigstr = readWriteFile();

   myt = getStruct(bigstr);

   printf("%d\t%d\t%d\n", myt->arrTimeinfo[0].tm_min, myt->arrTimeinfo[1].tm_min, myt->length);

/*Освобождаем выделенную память*/
    for(; i < 128; ++i) free(bigstr[i]);
    free(bigstr);
    free(myt);
    return 0;

 }

