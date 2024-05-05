#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define seed 42
#define MAXSIZE (11 + 256 + 256 * 256) * 1024 / sizeof(uint)

uint gen_next(uint elem) {
    return elem * 22695477 + 239; // mod is 2^32, using overflow as mod calculation
}

int main(int argc, char** argv) {
    char* filename = "big_file";
    uint data = seed;
    int size = atoi(argv[1]); // number of bits
    size /= sizeof(uint);
    if (size <= 0 || size > MAXSIZE) {
        fprintf(2, "Incorrect size\n");
        exit(1);
    }
    const int BUFSIZE = 2048;
    uint* buf = malloc(sizeof(uint) * BUFSIZE);
    if (buf == 0) {
        fprintf(2, "Memory error\n");
        exit(6);
    }
    int fd = open(filename, O_CREATE | O_WRONLY);
    if (fd < 0) {
        fprintf(2, "Cannot create file\n");
        exit(2);
    }
    for (int i = 0; i < size; ++i) {
        // printf("%d %d\n", i, data);
        buf[i % BUFSIZE] = data;
        // printf("%d\n", i);
        if (i == size - 1 || (i + 1) % BUFSIZE == 0) {
            int write_size = size % BUFSIZE;
            if ((i + 1) % BUFSIZE == 0) {
                write_size = BUFSIZE;
            }
            // printf("%d\n", write_size);
            if (write(fd, buf, sizeof(uint) * write_size) != sizeof(uint) * write_size) {
                close(fd);
                fprintf(2, "Incorrect write\n");
                exit(3);
            }
        }
        // printf("%d\n", i);
        data = gen_next(data);
    }
    close(fd);
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        fprintf(2, "Cannot open file\n");
        exit(4);
    }
    data = seed;
    for (int i = 0; i < size / BUFSIZE; ++i) {
        if (read(fd, buf, sizeof(uint) * BUFSIZE) != sizeof(uint) * BUFSIZE) {
            close(fd);
            fprintf(2, "Incorrect read\n");
            exit(5);
        }
        for (int j = 0; j < BUFSIZE; ++j) {
            if (data != buf[j]) {
                close(fd);
                printf("Error in position %d, readed %d, expected %d\n", i * BUFSIZE + j, buf[j], data);
                exit(0);
            }
            data = gen_next(data);
        }
    }
    if (size % BUFSIZE != 0) {
        if (read(fd, buf, sizeof(uint) * (size % BUFSIZE)) != sizeof(uint) * (size % BUFSIZE)) {
            close(fd);
            fprintf(2, "Incorrect read\n");
            exit(5);
        }
        for (int j = 0; j < size % BUFSIZE; ++j) {
            if (data != buf[j]) {
                close(fd);
                printf("Error in position %d, readed %d, expected %d\n", size - (size % BUFSIZE) + j, buf[j], data);
                exit(0);
            }
            data = gen_next(data);
        }
    }
    printf("All right\n");
    exit(0);
}