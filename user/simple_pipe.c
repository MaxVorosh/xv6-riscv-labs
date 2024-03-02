#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char** argv) {
    int p[2];
    pipe(p);
    const int buf_size = 21;
    int pid = fork();
    if (pid < 0) {
        const int msg_len = 24;
        write(2, "Can't start new process\n", msg_len);
        exit(1);
    }
    if (pid == 0) {
        close(p[1]);
        char buf[buf_size];
        while (read(p[0], buf, buf_size) > 0) {
            printf("%s", buf);
        }
        close(p[0]);
        exit(0);
    }
    close(p[0]);
    char buf[buf_size];
    int status;
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        if (len >= buf_size) {
            close(p[1]);
            wait(&status);
            const int msg_len = 16;
            write(2, "Buffer overflow\n", msg_len);
            exit(2);
        }
        strcpy(buf, argv[i]);
        buf[len] = '\n';
        write(p[1], buf, len + 1);
    }
    close(p[1]);
    wait(&status);
    exit(0);
}