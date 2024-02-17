#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64 sys_add(void) {
    int num1, num2;
    argint(0, &num1);
    argint(1, &num2);
    printf("%d\n", num1 + num2);
    return 0;
}