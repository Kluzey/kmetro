#include <stdatomic.h>

typedef struct {
    atomic_bool ready;
    long long interval;
} timerData;

void* timer(void* arg);