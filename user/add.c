#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    const int buf_size = 20;
    char buf[buf_size];
    gets(buf, buf_size);
    char* end_buf = buf + buf_size - 1;
    char* second_number_ptr = buf;
    while (second_number_ptr != end_buf) {
        if (*second_number_ptr == ' ') {
            break;
        }
        if (*second_number_ptr == '\n' || *second_number_ptr == '\0') {
            // Only one number
            printf("Only one number given\n");
            exit(1);
        }
        if ('0' <= *second_number_ptr && *second_number_ptr <= '9') {
            second_number_ptr++;
        }
        else {
            // Bad symbol
            printf("Not a number\n");
            exit(3);
        }
    }
    second_number_ptr++;
    if (second_number_ptr >= end_buf) {
        printf("Buffer overflow\n");
        exit(2);
    }
    int num1 = atoi(buf);
    int num2 = atoi(second_number_ptr);
    int sum = num1 + num2;
    printf("%d\n", sum);
    exit(0);
}
