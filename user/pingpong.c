#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int parent_fd[2], child_fd[2];
    char buf[10];

    if(pipe(parent_fd) < 0 || pipe(child_fd) < 0) {
        fprintf(2, "Error: Can't create pipe!\n");
        exit(1);
    }
    int pid = fork();

    if(pid == 0) {  // Tiến trình con
        close(parent_fd[1]);
        close(child_fd[0]);
        read(parent_fd[0], buf, 4);  // Đọc 4 ký tự
        if(strcmp(buf, "ping") == 0) {
            printf("%d: received ping\n", getpid());
        }
        write(child_fd[1], "pong", 4);
        close(parent_fd[0]);
        close(child_fd[1]);
    } else {  // Tiến trình cha
        close(parent_fd[0]);
        close(child_fd[1]);
        write(parent_fd[1], "ping", 4);
        read(child_fd[0], buf, 4);  // Đọc 4 ký tự
        if(strcmp(buf, "pong") == 0) {
            printf("%d: received pong\n", getpid());
        }
        close(parent_fd[1]);
        close(child_fd[0]);
    }
    exit(0);
}
