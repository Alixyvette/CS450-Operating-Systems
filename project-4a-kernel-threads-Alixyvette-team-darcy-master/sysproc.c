#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_clone(void)
{
	void(*fnc)(void*,void*);
	void* arg1;
	void* arg2;
	void* stk; 
	// check for proper arguments as needed
	if(argptr(0,(char**)(&fnc), sizeof(void*) ) < 0)
		return -1;
	if(argptr(1,(char**)(&arg1), sizeof(void*) ) < 0)
		return -1;
	if(argptr(2,(char**)(&arg2), sizeof(void*) ) < 0)
		return -1;
	if(argptr(3,(char**)(&stk), sizeof(void*) ) < 0)
		return -1;

	return clone(fnc, arg1, arg2, stk);
/*

	int fnc;
	int arg1;
	int arg2;
	int stk;
	if(argint(0, &fnc) < 0)
		return -1;
	if(argint(1, &arg1) < 0)
		return -1;
	if(argint(2, &arg2) < 0)
		return -1;
	if(argint(3, &stk) < 0)
		return -1;
	
	return clone((void*)fnc, (void*)arg1, (void*)arg2, (void*)stk);
*/
}

int sys_join(void)
{
	void** stk;
	if(argptr(0,(char**)(&stk), sizeof(void*) ) < 0)
		return -1;
	// check for proper arguments as needed
	return join(stk);
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
