#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/mutex.h"

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("No arguments :(\n");
        exit(1);
    }
    int to_child[2];
    int to_parent[2];
    int child_stat = pipe(to_child);
    int parent_stat = pipe(to_parent);
    if (child_stat < 0 || parent_stat < 0) {
        printf("Can't create pipe\n");
        exit(2);
    }
    int mut = createmutex();
    if (mut < 0) {
        printf("Can't create mutex\n");
    }
    int pid = fork();
    if (pid < 0) {
        printf("Can't create new process\n");
        exit(3);
    }
    if (pid == 0) {
        close(to_child[1]);
        close(to_parent[0]);
        char c;
        int n;
        int my_pid = getpid();
        while ((n = read(to_child[0], &c, 1)) > 0) {
            lock(mut);
            printf("%d: received %c\n", my_pid, c);
            unlock(mut);
            write(to_parent[1], &c, 1);
        }
        close(to_child[0]);
        close(to_parent[1]);
        if (n < 0) {
            lock(mut);
            printf("Reading error\n");
            unlock(mut);
            exit(4);
        }
        exit(0);
    }
    close(to_child[0]);
    close(to_parent[1]);
    write(to_child[1], argv[1], strlen(argv[1]));
    close(to_child[1]);
    char c;
    int n;
    int my_pid = getpid();
    while ((n = read(to_parent[0], &c, 1)) > 0) {
        lock(mut);
        printf("%d: received %c\n", my_pid, c);
        unlock(mut);
    }
    close(to_parent[0]);
    if (n < 0) {
        lock(mut);
        printf("Reading error\n");
        unlock(mut);
        exit(4);
    }
    int status;
    wait(&status);
    exit(0);
}