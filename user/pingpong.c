#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main() {
    char buf[10];
    int pipe1[2], pipe2[2];
    pipe(pipe1);  // child to parent 
    pipe(pipe2);  // parent to child
    if (fork() == 0) {  // child process
        read(pipe2[0], buf, 1);
        fprintf(0, "%d: received ping\n", getpid());
        write(pipe1[1], buf, 1);
        exit(0);
    } else {  // parent process
        write(pipe2[1], "s", 1);
        read(pipe1[0], buf, 1);
        fprintf(0, "%d: received pong\n", getpid());
        exit(0);
    }
}