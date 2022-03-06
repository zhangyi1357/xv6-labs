#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>

void next_prime(int *p) {
    close(p[1]);
    uint32 prime;
    if (read(p[0], &prime, 4) == 4) {
        printf("prime %d\n", prime);
        int next_p[2];
        pipe(next_p);
        if (fork() > 0) {
            close(next_p[0]);
            int n;
            while (read(p[0], &n, 4) == 4) {
                if (n % prime != 0)
                    write(next_p[1], &n, 4);
            }
            close(p[0]);
            close(next_p[1]);
            wait(0);
        } else {
            close(p[0]);
            next_prime(next_p);
        }
    }
}

int main() {
    int p[2];
    pipe(p);
    if (fork() > 0) {
        close(p[0]);
        for (int i = 2; i < 35; ++i)
            write(p[1], &i, 4);
        close(p[1]);
        wait(0);
    } else
        next_prime(p);
    exit(0);
}