#include "header.h"

#define SIZE 128

int main(){
    int i = 0;
    char ** bigstr;
    mt * myt;
    writeFile();
    bigstr = readFile();

    myt = getStruct(bigstr);

   printf("%d\t%s\t%d\n", myt[0].arrTimeinfo.tm_min, myt[0].message, myt[0].length);

/*Освобождаем выделенную память*/
    for(; i < 128; ++i) free(bigstr[i]);
    free(bigstr);
    free(myt);
    return 0;

 }

