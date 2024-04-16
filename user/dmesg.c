#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/msg_buf.h"

int main() {
    char* buf = malloc(BUFCNT * PGSIZE + 1);
    buf[BUFCNT * PGSIZE] = 0;
    chlog(SWITCH, 1);
    chlog(EXEC, 1);
    chlog(TRAP, 1);
    chlog(SYSCALL, 1);
    chtime(100);
    if (dmesg((uint64)buf) < 0) {
        printf("Wrong address\n");
        return 1;
    }
    printf("%s", buf);
    free(buf);
    return 0;
}