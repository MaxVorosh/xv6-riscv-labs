#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char** argv) {
    int p[2];
    pipe(p);
    const int buf_size = 100;
    int pid = fork();
    if (pid < 0) {
        int msg_len = 24;
        write(2, "Can't start new process\n", msg_len);
        exit(1);
    }
    if (pid == 0) {
        close(p[1]);
        char buf[buf_size];
        read(p[0], buf, buf_size);
        printf("%s", buf);
        close(p[0]);
        exit(0);
    }
    close(p[0]);
    char buf[buf_size];
    char* ptr = buf;
    int sum_len = 0;
    for (int i = 1; i < argc; ++i) {
        strcpy(ptr, argv[i]);
        int len = strlen(argv[i]) + 1;
        sum_len += len;
        ptr += len;
    }
    for (int i = 0; i < sum_len; ++i) {
        if (buf[i] == '\0') {
            buf[i] = '\n';
        }
    }
    buf[sum_len] = '\0';
    write(p[1], buf, sum_len);
    close(p[1]);
    int status;
    wait(&status);
    exit(0);
}