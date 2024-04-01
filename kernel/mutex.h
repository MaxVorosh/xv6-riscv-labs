#include "spinlock.h"
#include "sleeplock.h"


typedef struct mutex {
    struct spinlock access_lock;

    // Only if access_lock acquired
    struct sleeplock lock;
    int times;
    int pid;
} mutex_t;

int removemutex(int mutex_desc);