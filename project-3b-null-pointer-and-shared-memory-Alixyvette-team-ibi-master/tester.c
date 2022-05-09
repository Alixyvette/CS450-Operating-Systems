#include <types.h>
#include <user.h>

static unsigned int get_null (void) {
	return sleep(0);
}

int main (int argc, char * argv[])
{
	unsigned int *p = (unsigned int*)get_null();
	printf(1,"%d --- %x\n", (int)*p,(int)*p);
	//return 0;
	exit();
}
