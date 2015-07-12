
#include "header.h"

#define SIZE 128
/*
int a = 5;
char ** readWriteFile() {

    char str[SIZE];
    int i = 0, j = 0;

    char ** bigstr = (char **)malloc(128 * sizeof(char *));
    for(; i < 128; ++i) bigstr[i] = (char *)malloc(128 * sizeof(char *));
    i = 0;
    FILE * f;
    time_t rawtime;
    struct tm * timeinfo;

    f = fopen("time.txt", "r");
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("Чем планируешь заниматься?\n");
    gets(str);

    fprintf(f, "%02d:%02d %02d-%02d-%04d\t%s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year + 1900, str);

    while (fgets (str, SIZE, f) != NULL)  {
            for(; str[i] != '\n'; ++i )
                bigstr[j][i] = str[i];
            j++, i = 0;
    }

    fclose(f);
    return bigstr;
}

 mt * getStruct( char ** bigstr ) {
     int i = 0;
     mt * myt;
     if((myt = malloc(sizeof(mt)))==NULL) {
         printf("Ошибка при распределении памяти\n");
         exit(1);
      }

     for(i = 0; strlen(bigstr[i]) != 0; ++i) {
         myt->arrTimeinfo[i].tm_hour = atoi(&bigstr[i][0]);
         myt->arrTimeinfo[i].tm_min = atoi(&bigstr[i][3]);
         myt->arrTimeinfo[i].tm_mday = atoi(&bigstr[i][5]);
         myt->arrTimeinfo[i].tm_mon = atoi(&bigstr[i][9]);
         myt->arrTimeinfo[i].tm_year = atoi(&bigstr[i][12]) - 1900;
         myt->length = i + 1;
     }
     return myt;
}

*/
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

