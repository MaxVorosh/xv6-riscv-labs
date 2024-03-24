#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "proc.h"
#include "defs.h"

mutex_t mutex_table[NMUTEX];

void mutexesinit() {
    for (mutex_t* m = mutex_table; m < &mutex_table[NMUTEX]; ++m) {
        initlock(&m->access_lock, "safety lock");
        initsleeplock(&m->lock, "working lock");
        m->times = 0;
    }
}

int add_mutex_to_process(mutex_t* m, struct proc* p) {
    acquire(&p->lock);
    for (int i = 0; i < PROCMUTEX; ++i) {
        if (p->mutexes[i] == 0) {
            p->mutexes[i] = m;
            release(&p->lock);
            return i;
        }
    }
    release(&p->lock);
    return -1;
}

int createmutex() {
    struct proc* p = myproc();
    for (mutex_t* m = mutex_table; m < &mutex_table[NMUTEX]; ++m) {
        acquire(&m->access_lock);
        if (m->times == 0) {
            m->times = 1;
            int descriptor = add_mutex_to_process(m, p);
            release(&m->access_lock);
            return descriptor;
        }
        release(&m->access_lock);
    }
    return -2;
}

int lock(int mutex_desc) {
    if (mutex_desc >= PROCMUTEX || mutex_desc < 0) {
        // Wrong descriptor
        return -1;
    }
    struct proc* p = myproc();
    acquire(&p->lock);
    mutex_t* m = p->mutexes[mutex_desc];
    release(&p->lock);
    if (m == 0) {
        // Wrong descriptor
        return -1;
    }
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
    struct proc* p = myproc();
    acquire(&p->lock);
    mutex_t* m = p->mutexes[mutex_desc];
    release(&p->lock);
    if (m == 0) {
        // Wrong descriptor
        return -1;
    }
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
    struct proc* p = myproc();
    acquire(&p->lock);
    mutex_t* m = p->mutexes[mutex_desc];
    p->mutexes[mutex_desc] = 0;
    release(&p->lock);
    if (m == 0) {
        // Wrong descriptor
        return -1;
    }
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