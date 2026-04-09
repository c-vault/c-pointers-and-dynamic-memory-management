/* dangling1.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "memory.h"

extern char *get_dynamic_str();

void main()
{
	int i=0;
	char *string1=NULL,*string2=NULL,*string3=NULL;
	char *stringp=NULL;
	
	printf("Enter first string: ");
	fflush(stdout);
	string1 = get_dynamic_str();
	
	stringp = string1;
	printf("stringp (%p) before dangling is %s.\n",stringp,stringp);

	printf("Enter second string: ");
	fflush(stdout);
	string2 = get_dynamic_str();
	free(string1);

	printf("Enter third string: ");
	fflush(stdout);
	string3 = get_dynamic_str();
	memory_map();
	free(string2);
	free(string3);
	
	printf("stringp (%p) after dangling points to %s.\n",stringp, stringp);
	memory_map();
}
	