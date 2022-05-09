#include "types.h"
#include "user.h"
#include "x86.h"

#define assert(x) if (x) { /* pass */ } else { \
   printf(1, "assert failed %s %s %d\n", #x , __FILE__, __LINE__); \
   exit(); \
   }


int
main(int argc, char *argv[])
{
  int rc = getreadcount();
  assert(rc > 0);
  printf(1, "PASSED TEST!\n");
  exit();
}
