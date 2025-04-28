/* declare.c */
/* Before we declare our header files, let me say that when you use a lot of pointers in your programs you may want to get into the habit of always including the following four header files.  */
#include <stdio.h> /* common input-output routines */
#include <stdlib.h> /* the dynamic memory management routines */
#include <ctype.h> /* the isalpha, ispunct ... character checking routines. */
#include <string.h> /* string manipulation routines */
/* on an IBM PC, use quotes " instead of <> and you will also need malloc.h */
void main()
{
	/* first let's declare some basic data types.  So you can compare basic data type 	declaration to pointer declaration */	
	char mychar;
	int myint;

	/* now to declare a pointer, you specify the data type your variable will be 	pointing to and then put an asterisk (*) in front of your variable name */
	char *char_ptr;
	int *int_ptr;
	void *void_ptr;

	/* since we just wanted to learn how to declare pointers this program doesn't 	need to do anything else. */
}
