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

// Size of the variables array.
#define MAX_VARIABLES 32

typedef struct sh_variable {
 char *name;
 char *value;
} sh_variable;

struct sh_variable global_variables[MAX_VARIABLES];
int next_index = 0;

/**
 * Sets a variable in the global variables list.
 */
int sys_setvariable(char* variable_name, char* variable_value) {
  if (next_index >= MAX_VARIABLES) {
    // No more room.
    return -1;
  }

  // Copy strings into struct.
  struct sh_variable *new_var = &global_variables[next_index];
  new_var->name = variable_name;
  new_var->value = variable_value;

  // Set this in the array.
  global_variables[next_index] = *new_var;
  next_index++;
  return 0;
}

/**
 * Gets a variable by name.
 * @param  variable_name  variable name to search for.
 * @param  variable_value pointer to the value (as reutrn value)
 * @return          0 on success, -1 on not variable found.
 */
int sys_getvariable(char* variable_name, char* variable_value) {
  for (int index = 0; index < next_index; index++) {
    if (strncmp((&global_variables[index])->name, variable_name, strlen(variable_name)) == 0) {
      // Found the variable.
      variable_value = (&global_variables[index])->value;
      return 0;
    }
  }
  // Not found.
  return -1;
}

int sys_remvariable(char* variable_name) {
  return -1;
}
