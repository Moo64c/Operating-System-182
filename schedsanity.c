#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void addpid(int newpid, int *pidlist, int *nextindex);
void schedsanity1();


void
schedsanity(int calc_type) {
  switch(calc_type) {
    // Run according to type.
    case 1:
      schedsanity1();
      break;

    default:
      return;
  }
}

// 1 - These processes will perform a simple calculation within a medium sized loop
void schedsanity1() {
  int number_of_children = 5;
  //int pids[number_of_children];
  //int nextlistedpid = 0;
  int curpid = 0;
  int somepid = -1;

  int result = 0;

  for (int index = 0; index < number_of_children; index++) {
    curpid = fork();
    if (curpid < 0) {
      // error.
      printf(2, "error in fork()");
      exit();
    }
    else if (curpid == 0) {
      // Child. Check PID is in the list.
      somepid = getpid();
      //addpid(somepid, pids, &nextlistedpid);

      // Simple calculation in a medium sized loop.
      for(int interiorIndex = 0; interiorIndex < 500; interiorIndex++) {
        result++;
      }
      printf(2, "result for pid %d : %d\n", somepid, result);
      return;
    }
    else {
      // Parent.
      int t1; int t2; int t3;
      if (somepid > 0) {
        wait2(somepid, &t1, &t2, &t3);
      }
    }
  }
  // 2 - These processes will perform simple calculation within a very large loop
  // 3 - These processes will perform printing to screen within a medium sized loop
  // 4 - These processes will perform printing to screen within a very large loop
}

// Adds pid to list (if doesn't exist).
void addpid(int newpid, int *pidlist, int *nextindex) {
  for (int index = 0; index < (*nextindex); index++) {
    if (*(pidlist + (sizeof(int) * index) ) == newpid) {
      return;
    }
  }
  // Not found, add.
  *(pidlist + (((unsigned int) nextindex) * sizeof(int))) = newpid;
  (*nextindex)++;
  printf(2, "added pid %d to list\n", newpid);
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
