#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


void
schedsanity(int calc_type) {
  // Run according to type.
  // 1 - These processes will perform a simple calculation within a medium sized loop
  if (calc_type == 1) {
    int pid = 0;
    int somepid = -1;
    for (int index = 0; index < 5; index++) {
      pid = fork();
      if (pid == 0) {
        // child.
        somepid = getpid();
        printf(2, "test child %d\n", somepid);
      }
      else {
        printf(2, "parentttt\n");
        int t1; int t2; int t3;
        if (somepid > 0) {
          wait2(somepid, &t1, &t2, &t3);
          printf(2, "pid %d, %d - %d - %d\n", somepid, &t1,&t2, &t3);
        }
      }
    }
  }
  // 2 - These processes will perform simple calculation within a very large loop
  // 3 - These processes will perform printing to screen within a medium sized loop
  // 4 - These processes will perform printing to screen within a very large loop
}

int
main(int argc, char *argv[])
{
  schedsanity(1);
  schedsanity(2);
  schedsanity(3);
  schedsanity(4);
  exit();
}
