#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

int 
main() {
    int buf_size = 8;
    int success = 0;
    int processes = 0;
    char *states[] = {
        [IUNUSED]    "unused",
        [IUSED]      "used",
        [ISLEEPING]  "sleep ",
        [IRUNNABLE]  "runble",
        [IRUNNING]   "run   ",
        [IZOMBIE]    "zombie"
    };
    while (!success) {
        procinfo_t buf[buf_size];
        processes = pcinfo((uint64)buf, buf_size);
        if (processes == -1) {
            fprintf(2, "Memory error\n");
            exit(1);
        }
        if (processes <= buf_size) {
            success = 1;
            for (int i = 0; i < processes; ++i) {
                printf("Name: %s, Parent PID: %d, State: %s\n", buf[i].name, buf[i].parent_id, states[buf[i].state]);
            }
        }
        buf_size *= 2;
    }
    exit(0);
}