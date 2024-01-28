#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

static char new_name[DIRSIZ+1];

char* rm_back_space(char* name) {
    int i=strlen(name)-1;
    while(name[i] == ' ') { i--; }
    int k = 0;
    for(; k<=i; k++) {
        new_name[k] = name[k];
    }
    new_name[k] = '\0';
    return new_name;

}

void find(char* path, char* name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
        case T_DEVICE:
        case T_FILE:
            fprintf(2, "find: find needs a path.\n");
            close(fd);//
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                fprintf(2, "find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if(de.inum == 0) continue;
                if(strcmp(".", rm_back_space(de.name)) == 0 || strcmp("..", rm_back_space(de.name)) == 0) continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = '\0';
                if(stat(buf, &st) < 0) {
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }
                if(st.type == T_DEVICE || st.type == T_FILE) {
                    if(strcmp(name, rm_back_space(de.name)) == 0) {
                        printf("%s\n", buf);
                    }
                }
                else if(st.type == T_DIR) {
                    find(buf, name);
                }
            }
    }
    
}


int main(int argc, char* argv[]) {
    if(argc != 3) {
        printf("find:find needs 3 arguments, but you provide %d.\n", argc);
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}