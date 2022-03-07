#include <kernel/types.h>
#include <kernel/stat.h>
#include <kernel/param.h>
#include <user/user.h>
// implement xargs for MIT 6.S081
int main(int argc, char* argv[]) {
    char argv_copy[20][MAXARG];

    for (int i = 1; i < argc; ++i) {
        strcpy(argv_copy[i - 1], argv[i]);
    }

    int num_args = argc - 1;

    char ch;
    int i = 0;
    while (read(0, &ch, 1) == 1) {
        if (ch != '\n' && ch != ' ') {
            argv_copy[num_args][i++] = ch;
        } else {
            argv_copy[num_args++][i] = '\0';
            i = 0;
        }
    }

    argv_copy[num_args][i] = '\0';

    // for (int i = 0; i < num_args; ++i) {
    //     printf("%s\n", argv_copy[i]);
    // }

    // exit(0);

    char* argv_for_exec[100];
    for (int i = 0; i < num_args; ++i) {
        argv_for_exec[i] = argv_copy[i];
    }

    if (fork() == 0) {
        exec(argv_for_exec[0], argv_for_exec);
        exit(0);
    } else {
        wait(0);
        exit(0);
    }
}