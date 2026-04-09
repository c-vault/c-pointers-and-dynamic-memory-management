/* dangling2.c */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

char *bad_initialize()
{
	char buffer[128];
	
	printf("Enter string: ");
	fflush(stdout);
	gets(buffer);
	
	return(buffer);
}

void main()
{
	char *names[5];
	int i;
	
	for (i=0; i < 5; i++)
		names[i] = bad_initialize();
		
	for (i=0; i < 5; i++)
		printf("name[%d] is %s.\n",i,names[i]);
}