#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char** argv) {
    int pid = fork();
    if (pid < 0) {
        const int msg_len = 24;
        write(2, "Can't start new process\n", msg_len);
        exit(2);
    }
    if (pid == 0) {
        sleep(100);
        exit(1);
    }
    printf("Parent id: %d, Child id: %d\n", getpid(), pid);
    char* kill_flag = "-b";
    if (argc > 1 && strcmp(argv[1], kill_flag) == 0) {
        int kill_result = kill(pid);
        if (kill_result < 0) {
            const int msg_len = 27;
            write(2, "Can't kill a child process\n", msg_len);
            exit(3);
        }
    }
    int status;
    int cpid = wait(&status);
    printf("Ended id: %d, Status: %d\n", cpid, status);
    exit(0);
}