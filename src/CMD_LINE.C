/* cmd_line.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main(int argc, char *argv[])
{
	int i,j;

	char **my_arg=NULL;

	my_arg = argv;
	
	/* print out as strings */
	for (i=0; i<argc; i++)
		printf("argv[%d] = %s.\n",i,my_arg[i]);
		
	/* print out as characters */
	for (i=0; i<argc; i++)
	{
		/* wouldn't it have been easier if they made the
		argv pointer pointer NULL terminated? */
		for (j=0; *((*(my_arg+i)) + j); j++)
			printf("character[%d] of the row[%d] is %c.\n",j,i,my_arg[i][j]);
	}
	
}