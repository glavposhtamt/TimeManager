#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "header.h"

#define SIZE 128

int main(){
    typedef struct myTymeInfo mt;
    char str[SIZE];
    int i = 0, j = 0;
    char bigstr[SIZE][SIZE];
    FILE * f;
    mt myt;
    time_t rawtime;
    struct tm * timeinfo;

    f = fopen("time.txt", "r");
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("Чем планируешь заниматься?\n");
    gets(str);

    fprintf(f, "%02d:%02d %02d-%02d-%04d\t%s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year + 1900, str);

    while (fgets (str, SIZE, f) != NULL)  {
            for(; str[i] != '\n'; ++i ) bigstr[j][i] = str[i];
            j++, i = 0;
    }

    fclose(f);

    for(i = 0; strlen(bigstr[i]) != 0; ++i) {
        myt.arrTimeinfo[i].tm_hour = atoi(&bigstr[i][0]);
        myt.arrTimeinfo[i].tm_min = atoi(&bigstr[i][3]);
        myt.arrTimeinfo[i].tm_mday = atoi(&bigstr[i][5]);
        myt.arrTimeinfo[i].tm_mon = atoi(&bigstr[i][9]);
        myt.arrTimeinfo[i].tm_year = atoi(&bigstr[i][12]) - 1900;
        myt.length = i + 1;
    }

    printf("%d\t%d\t%d\n", myt.arrTimeinfo[0].tm_min, myt.arrTimeinfo[1].tm_min, myt.length);

    return 0;
 }

