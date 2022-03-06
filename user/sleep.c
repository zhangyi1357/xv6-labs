#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

int main(int argc, char *argv[]) {
    int sleep_time;
    if (argc < 1) {
        fprintf(2, "Need an argument time to sleep!");
        exit(1);
    }
    sleep_time = atoi(argv[1]);
    int pid = fork();
    if (pid == 0) {
        sleep(sleep_time);
        exit(0);
    }
    int status;
    wait(&status);
    if (status != 0)
        fprintf(2, "Sys sleep failed!");
    exit(0);
}