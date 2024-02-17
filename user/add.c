#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ONE_NUMB -1
#define BUF_OVER -2
#define BAD_SYMB -3
#define READ_ERR -4
#define EMPTY_STR -5

int read_number(char* buf, int buf_size, int is_last) {
    int i = 0;
    while (i < buf_size) {
        int res = read(0, buf + i, 1);
        if (res < 0) {
            return READ_ERR; // Error in reading
        }
        if (res == 0 || buf[i] == ' ') {
            break;
        }
        if (buf[i] == '\n' || buf[i] == '\0') {
            if (is_last == 1) {
                break;
            }
            if (i == 0) {
                return EMPTY_STR;
            }
            return ONE_NUMB; // Next number will not be read
        }
        if (!('0' <= buf[i] && buf[i]  <= '9')) {
            return BAD_SYMB; // Bad symbol
        }
        ++i;
    }
    if (i == 0) {
        if (is_last == 0) {
            return EMPTY_STR;
        }
        else {
            return ONE_NUMB;
        }
    }
    if (i == buf_size) {
        return BUF_OVER; // Bufer overflow
    }
    buf[i] = '\0';
    return 0;
}

void handle(int err) {
    if (err == ONE_NUMB) {
        write(2, "Only one number given\n", 22);
    }
    else if (err == BAD_SYMB) {
        write(2, "Not a number\n", 13);
    }
    else if (err == BUF_OVER) {
        write(2, "Buffer overflow\n", 16);
    }
    else if (err == READ_ERR) {
        write(2, "Reading error\n", 14);
    }
    else if (err == EMPTY_STR) {
        write(2, "Empty input\n", 12);
    }
}

int
main(int argc, char *argv[])
{
    const int buf_size = 11;
    char buf1[buf_size];
    char buf2[buf_size];
    int res = read_number(buf1, buf_size, 0);
    if (res < 0) {
        handle(res);
        exit(-res);
    }
    res = read_number(buf2, buf_size, 1);
    if (res < 0) {
        handle(res);
        exit(-res);
    }
    printf("|%s %s|\n", buf1, buf2);
    int num1 = atoi(buf1);
    int num2 = atoi(buf2);
    printf("%d\n", num1 + num2);
    exit(0);
}