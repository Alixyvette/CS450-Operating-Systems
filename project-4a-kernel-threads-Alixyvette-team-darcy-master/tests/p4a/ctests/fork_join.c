/*
 * after fork(), join() should not succeed
 * Authors:
 * - Varun Naik, Spring 2018
 * - Inspired by a test case from Spring 2016, last modified by Akshay Uttamani
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

int
main(int argc, char *argv[])
{
  void *stack;
  int var = 0;
  int pid;

  ppid = getpid();
  check(ppid > 2, "getpid() failed");

  pid = fork();
  check(pid >= 0, "fork() failed");

  if (pid == 0) {
    // Child process
    pid = getpid();
    check(pid > ppid, "fork() failed");
    ++var;
    exit();
    check(0, "Continued after exit");
  }

  // Parent process
  check(pid > ppid, "fork() failed");

  pid = join(&stack);
  check(pid == -1, "join() returned the wrong pid");

  pid = wait();
  check(pid > ppid, "wait() failed");
  check(var == 0, "var is incorrect");

  printf(1, "PASSED TEST!\n");
  exit();
}
