#include <stdio.h>
#include <stdlib.h>

struct fooalign {char x; double d;};
#define DEFAULT_ALIGNMENT ( (char *) & ((struct fooalign *) 0)->d - (char *) 0)

union fooround { long x; double d;};
#define DEFAULT_ROUNDING (sizeof (union fooround))

void main()
{
	printf("For this machine, DEFAULT_ALIGNMENT IS %ld.\n",DEFAULT_ALIGNMENT);
	printf("                  DEFAULT_ROUNDING IS %ld.\n",DEFAULT_ROUNDING);
}