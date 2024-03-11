#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/procinfo.h"

int
main() {
    printf("Correct call\n");
    procinfo_t* buf[64];
    int res = pcinfo((uint64)buf, 64);
    printf("Returned code: %d\n", res);
    printf("Small buffer\n");
    procinfo_t* small_buf[1];
    res = pcinfo((uint64)small_buf, 1);
    printf("Returned code: %d\n", res);
    printf("Nullptr\n");
    res = pcinfo(0, 64);
    printf("Returned code: %d\n", res);
    printf("Wrong address\n");
    res = pcinfo(-1, 64);
    printf("Returned code: %d\n", res);
    exit(0);
}