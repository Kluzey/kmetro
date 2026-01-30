#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    atomic_bool ready;
    long long interval;
} timerData;

void normaliseTs(struct timespec *t) {
    if (t->tv_nsec >= 1000000000L) {
        t->tv_sec += t->tv_nsec / 1000000000L;
        t->tv_nsec %= 1000000000L;
    } else if (t->tv_nsec < 0) {
        long long s = (-(long long)t->tv_nsec + 999999999LL) / 1000000000LL;
        t->tv_sec -= s;
        t->tv_nsec += s * 1000000000L;
    }
}

void* timer(void *args)
{
    timerData *arguements = args;
    struct timespec now, target;

    while(1){
        
        clock_gettime(CLOCK_MONOTONIC, &now);
        now.tv_nsec += (long long)arguements->interval;
        normaliseTs(&now);

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &now, NULL);
        atomic_store(&arguements->ready, true);
    }
    return 0;
}