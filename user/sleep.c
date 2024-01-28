#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("sleep : sleep need an argument.\n");
    exit(1);
  }
  if(argc > 2) {
    printf("sleep : sleep only needs one argument.\n");
    exit(1);
  }
  sleep(atoi(argv[1]));
  exit(0);
}