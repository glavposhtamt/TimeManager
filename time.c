#include "time.h"

cl secToTime(double seconds){
    long s = (long)seconds;
    int allMin;
    cl hms;

    hms.sec = s >= 60 ? s % 60 : s;
    allMin = s / 60;
    hms.min = allMin >= 60 ? allMin % 60 : allMin;
    hms.hours = allMin / 60;

    return hms;
}
