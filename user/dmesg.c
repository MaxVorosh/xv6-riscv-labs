#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"

int main() {
    char buf[BUFCNT * PGSIZE];
    if (dmesg((uint64)buf) < 0) {
        printf("Wrong address\n");
        return 1;
    }
    printf("%s\n", buf);
    return 0;
}