#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

void xargs_exec(char* program, char** paraments);

void
xargs(char** first_arg, int size, char* program_name)
{
    char buf[1024];
    
    char *arg[MAXARG];
    int m = 0;
    while (read(0, buf+m, 1) == 1) {
        if (m >= 1024) {
            fprintf(2, "xargs: arguments too long.\n");
            exit(1);
        }
        if (buf[m] == '\n') {
            buf[m] = '\0';
            memmove(arg, first_arg, sizeof(*first_arg)*size);
            // set a arg index
            int argIndex = size;
            if (argIndex == 0) {
                arg[argIndex] = program_name;
                argIndex++;
            }
            arg[argIndex] = malloc(sizeof(char)*(m+1));
            memmove(arg[argIndex], buf, m+1);
            // exec(char*, char** paraments): paraments ending with zero
            arg[argIndex+1] = 0;
            xargs_exec(program_name, arg);
            free(arg[argIndex]);
            m = 0;
        } else {
            m++;
        }
    }
}

void
xargs_exec(char* program, char** paraments)
{
    if (fork() > 0) {
        wait(0);
    } else {
        
        if (exec(program, paraments) == -1) {
            fprintf(2, "xargs: Error exec %s\n", program);
        }
    
    }
}

int
main(int argc, char* argv[]) 
{
    
    char *name = "echo";
    if (argc >= 2) {
        name = argv[1];
        
    }
    else {
    
    }
    xargs(argv + 1, argc - 1, name);
    exit(0);
}