#ifndef TIME
#define TIME

typedef struct Clock {
    int sec;
    int min;
    int hours;
} cl;

extern cl secToTime(double seconds);

#endif // TIME

