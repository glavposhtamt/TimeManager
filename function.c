#include "header.h"

void writeFile(){
    char str[SIZE];
    FILE * f;
    time_t rawtime;
    struct tm * timeinfo;
    f = fopen("time.txt", "a");

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    printf("Чем планируешь заниматься?\n");
    gets(str);

    fprintf(f, "%02d:%02d %02d-%02d-%04d\t%s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_mday, timeinfo->tm_mon, timeinfo->tm_year + 1900, str);
    fclose(f);
}

char ** readFile() {
    char str[SIZE];
    int i = 0, j = 0;
    FILE * fr;

    char ** bigstr = (char **)malloc(128 * sizeof(char *));
    for(; i < 128; ++i) bigstr[i] = (char *)malloc(128 * sizeof(char *));
    i = 0;

    fr = fopen("time.txt", "r");

    while (fgets (str, SIZE, fr) != NULL)  {
            for(; str[i] != '\n'; ++i )
                bigstr[j][i] = str[i];
            j++, i = 0;
    }


    fclose(fr);
    return bigstr;
}

mt * getStruct( char ** bigstr ) {
    int i = 0;
    for(;strlen(bigstr[i]) != 0;++i);
    mt * myt;
    if((myt = malloc(sizeof(mt) * i))==NULL) {
        printf("Ошибка при распределении памяти\n");
        exit(1);
     }

    for(i = 0; strlen(bigstr[i]) != 0; ++i) {
        myt[i].arrTimeinfo.tm_hour = atoi(&bigstr[i][0]);
        myt[i].arrTimeinfo.tm_min = atoi(&bigstr[i][3]);
        myt[i].arrTimeinfo.tm_mday = atoi(&bigstr[i][5]);
        myt[i].arrTimeinfo.tm_mon = atoi(&bigstr[i][9]);
        myt[i].arrTimeinfo.tm_year = atoi(&bigstr[i][12]) - 1900;
        myt[i].message = &bigstr[i][17];
        myt[i].length = i + 1;
    }
    return myt;
}
