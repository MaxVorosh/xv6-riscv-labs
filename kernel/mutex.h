#include "spinlock.h"
#include "sleeplock.h"


typedef struct mutex {
    struct spinlock access_lock;

    // Only if access_lock acquired
    struct sleeplock lock;
    int times;
} mutex_t;