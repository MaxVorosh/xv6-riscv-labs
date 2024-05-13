#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void check_res(int res, char* text) {
    if (res < 0) {
        fprintf(2, text);
        exit(1);
    }
}

void safe_mkdir(char* name) {
    int res = mkdir(name);
    check_res(res, "fs error\n");
}

void safe_chdir(char* name) {
    int res  = chdir(name);
    check_res(res, "fs error\n");
}

int safe_mkfile(char* name) {
    int fd = open(name, O_CREATE);
    check_res(fd, "fs error\n");
    return fd;
}

void safe_symlink(char* target, char* name) {
    int res = symlink(target, name);
    check_res(res, "symlink error\n");
}

void mkdirs() {
    safe_mkdir("p2");
    safe_mkdir("p2/p1");
    safe_mkdir("p2/p1/lns");
    safe_mkdir("p2/p1/lns/n1");
    safe_mkdir("p2/p1/lns/n1/n2");
}

int main() {
    mkdirs();
    char* one_path = "p2/p1/lns/1";
    char* two_path = "p2/2";
    char* three_path = "p2/p1/lns/n1/n2/3";
    int one = safe_mkfile(one_path);
    int two = safe_mkfile(two_path);
    int three = safe_mkfile(three_path);
    fprintf(one, "1");
    fprintf(two, "2");
    fprintf(three, "3");
    safe_symlink(one_path, "/p2/p1/lns/a1");
    safe_chdir("p2/p1/lns");
    safe_symlink("./1", "o1");
    safe_symlink("../../2", "o2");
    safe_symlink("n1/n2/3", "o3");
    char* argv[] = {0};
    exec("../../../ls", argv);
    fprintf(2, "exec fail\n");
    exit(2);
}