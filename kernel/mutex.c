#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "mutex.h"
#include "defs.h"

mutex_t mutex_table[NMUTEX];

void mutexesinit() {
    for (mutex_t* m = mutex_table; m < &mutex_table[NMUTEX]; ++m) {
        initlock(&m->access_lock, "safety lock");
        initsleeplock(&m->lock, "working lock");
        m->times = 0;
    }
}

int createmutex() {
    for (mutex_t* m = mutex_table; m < &mutex_table[NMUTEX]; ++m) {
        acquire(&m->access_lock);
        if (m->times == 0) {
            m->times = 1;
            release(&m->access_lock);
            return m - mutex_table;
        }
        release(&m->access_lock);
    }
    return -1;
}

int lock(int mutex_desc) {
    if (mutex_desc >= NMUTEX || mutex_desc < 0) {
        // Wrong descriptor
        return -1;
    }
    mutex_t* m = mutex_table + mutex_desc;
    acquire(&m->access_lock);
    if (holdingsleep(&m->lock)) {
        // Already locked
        m->times++;
        release(&m->access_lock);
        return 0;
    } 
    acquiresleep(&m->lock);
    m->times++;
    release(&m->access_lock);
    return 0;
}

int unlock(int mutex_desc) {
    if (mutex_desc >= NMUTEX || mutex_desc < 0) {
        // Wrong descriptor
        return -1;
    }
    mutex_t* m = mutex_table + mutex_desc;
    acquire(&m->access_lock);
    if (!holdingsleep(&m->lock)) {
        // Not locked
        release(&m->access_lock);
        return -2;
    }
    m->times--;
    if (m->times == 0) {
        releasesleep(&m->lock);
    }
    release(&m->access_lock);
    return 0;
}

int removemutex(int mutex_desc) {
    if (mutex_desc >= NMUTEX || mutex_desc < 0) {
        // Wrong descriptor
        return -1;
    }
    mutex_t* m = mutex_table + mutex_desc;
    acquire(&m->access_lock);
    if (holdingsleep(&m->lock)) {
        releasesleep(&m->lock);
    }
    m->times--;
    release(&m->access_lock);
    return 0;
}

int sys_createmutex(void) {
    return createmutex();
}

int sys_lock(void) {
    int descriptor;
    argint(0, &descriptor);
    return lock(descriptor);
}

int sys_unlock(void) {
    int descriptor;
    argint(0, &descriptor);
    return unlock(descriptor);
}

int sys_removemutex(void) {
    int descriptor;
    argint(0, &descriptor);
    return removemutex(descriptor);
}