#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    const int buf_size = 23;
    char buf[buf_size];
    gets(buf, buf_size);
    if (buf[0] == ' ' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {
        write(2, "Empty input\n", 12);
        exit(5);
    }
    char* end_buf = buf + buf_size - 1;
    char* second_number_ptr = buf;
    while (second_number_ptr != end_buf) {
        if (*second_number_ptr == ' ') {
            break;
        }
        if (*second_number_ptr == '\n' || *second_number_ptr == '\0' || *second_number_ptr == '\r') {
            // Only one number
            write(2, "Only one number given\n", 22);
            exit(1);
        }
        if ('0' <= *second_number_ptr && *second_number_ptr <= '9') {
            second_number_ptr++;
        }
        else {
            // Bad symbol
            write(2, "Not a number\n", 13);
            exit(3);
        }
    }
    second_number_ptr++;
    char* check_ptr = second_number_ptr;
    while (check_ptr < end_buf) {
        if (*check_ptr == '\0' || *check_ptr == '\n' || *check_ptr == '\r') {
            break;
        }
        if ('0' <= *check_ptr && *check_ptr <= '9') {
            check_ptr++;
        }
        else {
            write(2, "Not a number\n", 13);
            exit(3);
        }
    }
    if (check_ptr >= end_buf) {
        write(2, "Buffer overflow\n", 16);
        exit(2);
    }
    int num1 = atoi(buf);
    int num2 = atoi(second_number_ptr);
    int res = add(num1, num2);
    printf("%d\n", res);
    exit(0);
}
