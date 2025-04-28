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
	
	mychar = 'm';
	char_ptr = &mychar;
	
	printf("mychar is %c\n",mychar);
	printf("address of mychar is %p\n",&mychar);
	printf("char_ptr is %p\n",char_ptr);
	printf("char_ptr points to %c\n",*char_ptr);
}