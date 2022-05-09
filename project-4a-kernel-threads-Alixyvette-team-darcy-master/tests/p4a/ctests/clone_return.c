/*
 * test for clone() where the thread function returns, requires manual inspection
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
  // Change external state
  cpid = getpid();
  check(cpid > ppid, "getpid() returned the wrong pid");
  ++global;

  printf(1, "PID %d should trap to 0xffffffff...\n", getpid());

  // Return, rather than exit
  return;
}

void
crash(void)
{
  check(0, "Should not reach here");
}

int
main(int argc, char *argv[])
{
  void *stack1, *stack2;
  int pid1, pid2, status, i;

  ppid = getpid();
  check(ppid > 2, "getpid() failed");

  // Expand address space for stack
  stack1 = sbrk(PGSIZE);
  check(stack1 != (char *)-1, "sbrk() failed");
  check((uint)stack1 % PGSIZE == 0, "sbrk() return value is not page aligned");

  // Fill the thread stack with pointers to crash()
  for (i = 0; i < PGSIZE / sizeof(void *); ++i) {
    ((void (**)(void))stack1)[i] = &crash;
  }

  pid1 = clone(&func, NULL, NULL, stack1);
  check(pid1 > ppid, "clone() failed");

  pid2 = join(&stack2);
  status = kill(pid1);
  check(status == -1, "Child was still alive after join()");
  check(pid1 == pid2, "join() returned the wrong pid");
  check(stack1 == stack2, "join() returned the wrong stack");
  check(cpid == pid1, "cpid is incorrect");
  check(global == 1, "global is incorrect");
  printf(1, "PID %d joined. Check manually if the trap succeeded.\n", cpid);

  // Sleep for ~2 seconds so the output is visible on the screen...
  sleep(200);

  printf(1, "PASSED TEST!\n");
  exit();
}
