#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_yield(void)
{
  yield();
  return 0;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/**
 * Sets a variable in the global variables list.
 */
int sys_setVariable(void) {
  char * variable_name;
  argstr(0, &variable_name);
  char * variable_value;
  argstr(1, &variable_value);
  return setVariable(variable_name, variable_value);
}

/**
 * Gets a variable by name.
 * @param  variable_name  variable name to search for.
 * @param  variable_value pointer to the value (as reutrn value)
 * @return          0 on success, -1 on not variable found.
 */
int sys_getVariable(void) {
  char * variable_name;
  argstr(0, &variable_name);
  char * variable_value;
  argstr(1, &variable_value);
  return getVariable(variable_name, variable_value);
}

int sys_gprintvariables(void) {
  gprintvariables();
  return 0;
}
int sys_remVariable(void) {
  char* variable_name;
  argstr(0, &variable_name);
  return remVariable(variable_name);
}

int sys_wait2(void){
  int pid;
  int* wtime;
  int* rtime;
  int* iotime;

  if (argint(0, &pid)<0 || argptr(1, (void *)&wtime, sizeof(int*))<0 || argptr(2, (void *)&rtime, sizeof(int*))<0 || argptr(3, (void *)&iotime, sizeof(int*))<0)
    return -1;
  return wait2(pid, wtime, rtime, iotime);
}

int sys_set_priority(void){
  int priority;

  if (argint(0, &priority) <0 ) {
    return -1;
  }
  return set_priority(priority);
}
