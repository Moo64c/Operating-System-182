#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

void addpid(int newpid, int *pidlist, int *nextindex);
void schedsanity1();
void schedsanity2();


void
schedsanity(int calc_type) {
  switch(calc_type) {
    // Run according to type.
    case 1:
      schedsanity1();
      break;

    case 2:
      //schedsanity2();
      break;

    default:
      return;
  }
}

// 1 - These processes will perform a simple calculation within a medium sized loop
void schedsanity1() {
  int number_of_children = 50;
  int pids[number_of_children];
  int curpid;

  int result = 0;

  for (int index = 0; index < number_of_children; index++) {
    curpid = fork();
    if (curpid < 0) {
      // error.
      printf(2, "error in fork()");
      exit();
    }
    else if (curpid == 0) {
      // Child process.

      // Simple calculation in a medium sized loop.
      for(int interiorIndex = 0; interiorIndex < 500000; interiorIndex++) {
        if (interiorIndex % 2) {
          result += interiorIndex;
        }
        else {
          result += interiorIndex;
        }
      }
      exit();
    }
    else {
      // Parent.
      pids[index] = curpid;
    }
  }

  int wtime;
  int rtime;
  int iotime;
  for (int index = 0; index < number_of_children; index++) {
    wait2(pids[index], &wtime, &rtime, &iotime);
    printf(2, "result for pid %d : wtime %d rtime %d iotime %d\n", curpid, wtime, rtime, iotime);
  }
}

 // 2 - These processes will perform simple calculation within a very large loop
  void schedsanity2(){
  int number_of_children = 100;
  int result_of_fork = 0;

  for (int i =0 ; i< number_of_children ; i++){
    result_of_fork= fork();

    //fork() returns a zero to the newly created child process.
    if(result_of_fork == 0) {
      printf(2, "i'm a new child\n");
    }
    //fork() returns a positive value, the process ID of the child process, to the parent.
    else if (result_of_fork > 0){
      printf(2 , "i'm a parent and my child's process id is :  %d\n" , result_of_fork);

    }
    //ERROR - If fork() returns a negative value, the creation of a child process was unsuccessful.
    else if (result_of_fork < 0){
      printf(2, "fork() ERROR! \n");
    }
  }
  int calc = 8;
  while (calc < 900){
    calc = calc + 7;
  }


}


// 2 - These processes will perform simple calculation within a very large loop
// 3 - These processes will perform printing to screen within a medium sized loop
// 4 - These processes will perform printing to screen within a very large loop

int
main(int argc, char *argv[])
{
  schedsanity(1);
  schedsanity(2);
  schedsanity(3);
  schedsanity(4);
  exit();
}
