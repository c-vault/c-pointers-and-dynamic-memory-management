/* string_tst.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern char *strdup();
extern char *substr();

void main()
{
	char *dynamic_name=NULL;
	char *piece1=NULL, *piece2=NULL;
	
	char full_name[80];
	
	/* if you want to store a name in a character array.
	   You are limited to either knowing the correct size
	   or wasting space.  Even if you know the correct size
	   you cannot put a larger name in the array if the need
	   arises.  The best way to store data is in dynamic
	   structures! */
	
	/* old, inflexible way */
	strcpy(full_name,"Michael C. Daconta");
	
	/* better way */
	dynamic_name = strdup("Michael C. Daconta");
	
	printf("My full_name is %s and %s.\n",full_name,dynamic_name);
	
	/* if you want to change the name pointed to by full
	name, it is easy. */
	if (dynamic_name) free(dynamic_name);
	dynamic_name = NULL;
	dynamic_name = strdup("Frank Daconta");
	
	printf("The new full name is %s.\n",dynamic_name);
	
	/* let's just grab a piece of the string. */
	piece1 = substr(full_name,9,10);
	piece2 = substr(dynamic_name,1,5);
	
	printf("The pieces of my name are: %s %s.\n",piece2,piece1);

	/* cleanup */
	if (dynamic_name) free(dynamic_name);
	if (piece1) free(piece1);
	if (piece2) free(piece2);
}
	