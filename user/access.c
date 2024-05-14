#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 3000

void check_res(int res, int expected, int* data) {
    if (res == expected) {
        printf("OK\n");
    }
    else {
        fprintf(2, "Incorrect\n");
        free(data);
        exit(1);
    }
}

int main() {
    int* data = malloc(BUF_SIZE * sizeof(int));
    int res = accessed(data, sizeof(int) * BUF_SIZE);
    check_res(res, 1, data);

    data[2] = 1;
    res = accessed(data, sizeof(int) * BUF_SIZE);
    check_res(res, 1, data);
    res = accessed(data, sizeof(int) * BUF_SIZE);
    check_res(res, 0, data);

    data[10] = 0;
    res = accessed(data, sizeof(int) * 50);
    check_res(res, 1, data);
    res = accessed(data + 2950, sizeof(int) * 50);
    check_res(res, 0, data);

    data[2990] = 0;
    res = accessed(data, sizeof(int) * 50);
    check_res(res, 0, data);
    res = accessed(data + 2950, sizeof(int) * 50);
    check_res(res, 1, data);

    data[1024] = 1;
    res = accessed(&data[1024], sizeof(int));
    check_res(res, 1, data);
    res = accessed(&data[1025], sizeof(int));
    check_res(res, 0, data);
    res = accessed(&data[1023], sizeof(int));
    check_res(res, 0, data);

    printf("All tests passed\n");
    free(data);
    exit(0);
}