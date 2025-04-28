/* address_operator.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void main()
{
	char mychar;
	int myint;
	char *char_ptr;
	int *int_ptr;
	void *void_ptr;

	/* when you declare variables in a program (pointers included) the compiler 	sets aside the correct amount of memory for those variables.  Since the 	compiler has assigned memory locations (and addresses) to the variables you 	can be sure that declared variables have addresses.  To assign an address to a 	pointer variable you use an assignment statement and the address operator (&). 	*/
	mychar = 'm';
	char_ptr = &mychar;
	printf("mychar is %c.\n",mychar);
	printf("the address of mychar is at %p.\n",&mychar);
	printf("char_ptr is also %p.\n",char_ptr);
	printf("char_ptr points to %c.\n",*char_ptr);
}
