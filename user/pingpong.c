#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  int q[2];

  pipe(p);
  pipe(q);

  if(fork() == 0) {
    //child
    char ch; 
    if(read(p[0], &ch, 1) < 0) { exit(1); }
    close(p[0]);
    printf("%d: received ping\n", getpid());
    if(write(q[1], &ch, 1) < 0) { exit(1); }
    close(q[1]);
    exit(0);
  }
  else {
    //parent
    char ch = 'a';
    if(write(p[1], &ch, 1) < 0) { exit(1); }
    close(p[1]);
    wait(0);
    if(read(q[0], &ch, 1) < 0) { exit(1); }
    close(q[0]);
    printf("%d: received pong\n",getpid());
    exit(0);
  }
}