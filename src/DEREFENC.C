/* dereference.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void main()
{
	int myint, yourint;
	int *int_ptr;

	/* step one: assign the value 10 to myint */
	myint = 10;

	/* step two: assign myint's address to int_ptr */
	int_ptr = &myint;

	/* step three: assign the value of myint to yourint 
	     by dereferencing int_ptr */
	yourint = *int_ptr;

	/* print out the results */
	printf("myint is %d.\n",myint);
	printf("yourint is %d.\n",yourint);
}