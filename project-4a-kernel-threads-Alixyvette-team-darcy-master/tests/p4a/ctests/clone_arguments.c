/*
 * test for arguments to clone() and join()
 * Authors:
 * - Varun Naik, Spring 2018
 */
#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 0x1000
#define check(exp, msg) if(exp) {} else {\
  printf(1, "%s:%d check (" #exp ") failed: %s\n", __FILE__, __LINE__, msg);\
  printf(1, "TEST FAILED\n");\
  kill(ppid);\
  exit();}

int ppid = 0;
volatile int cpid = 0;
volatile int global = 0;

void
func(void *arg1, void *arg2)
{
  volatile int retval;

  // Assign retval to return value of clone()
  asm volatile("movl %%eax,%0" : "=r"(retval));

  check(*(int *)arg1 == 0xABCDABCD, "*arg1 is incorrect");
  check(*(int *)arg2 == 0xCDEFCDEF, "*arg2 is incorrect");
  check(((uint)&retval % PGSIZE) == 0xFE4, "Local variable is in wrong location");
  printf(1, "Retval is %d\n", retval);
  check(retval == 0, "Return value of clone() in child is incorrect");

  // Change external state
  *(int *)arg1 = 0x12341234;
  cpid = getpid();
  check(cpid > ppid, "getpid() returned the wrong pid");
  ++global;

  exit();

  check(0, "Continued after exit");
}

int
main(int argc, char *argv[])
{
  void *stack1, *stack2;
  int arg1 = 0xABCDABCD;
  int arg2 = 0xCDEFCDEF;
  int pid1, pid2, status;

  ppid = getpid();
  check(ppid > 2, "getpid() failed");

  // Expand address space for stack
  stack1 = sbrk(PGSIZE);
  check(stack1 != (char *)-1, "sbrk() failed");
  check((uint)stack1 % PGSIZE == 0, "sbrk() return value is not page aligned");

  pid1 = clone(&func, &arg1, &arg2, stack1);
  check(pid1 > ppid, "clone() failed");

  pid2 = join(&stack2);
  status = kill(pid1);
  check(status == -1, "Child was still alive after join()");
  check(pid1 == pid2, "join() returned the wrong pid");
  check(stack1 == stack2, "join() returned the wrong stack");
  check(arg1 == 0x12341234, "arg1 is incorrect");
  check(arg2 == 0xCDEFCDEF, "arg2 is incorrect");
  check(cpid == pid1, "cpid is incorrect");
  check(global == 1, "global is incorrect");

  printf(1, "PASSED TEST!\n");
  exit();
}
