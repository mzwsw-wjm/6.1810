#include "kernel/types.h"
#include "user/user.h"

void func(int p[2]) {
    int prime;
    close(p[1]);
    if(read(p[0], &prime, 4) == 4){
        int new_p[2];
        pipe(new_p);
        if(fork() == 0) {
            func(new_p);
        }
        else {
            close(new_p[0]);
            printf("prime %d\n",prime);
            int num;
            while(read(p[0], &num, 4) == 4) {
                if(num % prime) {
                    write(new_p[1], &num, 4);
                }
            }
            close(new_p[1]);
            close(p[0]);
            wait(0);
            exit(0);
        }
    }
}

int 
main(int argc, char* argv[])
{
    int p[2];
    pipe(p);
    if(fork() == 0) {
        //child
        func(p);
    }
    else {
        //parent
        close(p[0]);
        printf("prime 2\n");
        int i=3;
        for(; i<=35; i++) {
            if(i % 2) {
                if(write(p[1], &i, 4) < 4) { exit(1); }
            }
        }
        close(p[1]);
        wait(0);
        
    }
    exit(0);
}