#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char** argv) {
    int p[2];
    if (pipe(p) < 0) {
        const int msg_len = 18;
        write(2, "Can't create pipe\n", msg_len);
        exit(5);
    }
    int pid = fork();
    if (pid < 0) {
        const int msg_len = 24;
        write(2, "Can't start new process\n", msg_len);
        exit(1);
    }
    if (pid == 0) {
        close(p[1]);
        close(0);
        dup(p[0]);
        close(p[0]);
        char* argv[] = {"/wc", 0};
        exec("/wc", argv);
        const int msg_len = 14;
        write(2, "Can't exec wc\n", msg_len);
        exit(3);
    }
    close(p[0]);
    int status;
    for (int i = 1; i < argc; ++i) {
        int len = strlen(argv[i]);
        argv[i][len] = '\n';
        write(p[1], argv[i], len + 1);
    }
    close(p[1]);
    wait(&status);
    exit(0);
}