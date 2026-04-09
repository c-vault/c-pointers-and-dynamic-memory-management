/* stack_copy.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int add(int a, int b)
{
	int c;
	c = a + b;
	return(c);
}

void main()
{
	int cc, aa;
	aa = 20;
	cc = add(aa,80);
	printf("cc is %d.\n",cc);
}
