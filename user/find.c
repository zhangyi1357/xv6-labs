#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>
#include <kernel/fs.h>

char *fmtname(char *path) {
    // static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    return ++p;
}

void find(char *path, char *str) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }
    switch (st.type) {
        case T_FILE:
            // printf("%s %s\n", fmtname(path), str);
            if (strcmp(fmtname(path), str) == 0) {
                printf("%s\n", path);
            }
            break;
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0)
                    continue;
                if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;  // grantee null termination
                find(buf, str);
            }
            break;
        default:
            break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Need two arguments\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
