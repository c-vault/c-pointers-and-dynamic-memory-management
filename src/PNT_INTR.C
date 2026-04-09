/*  pointer_intro.c */
#include <stdio.h>
#include <stdlib.h>

void main()
{
	/* declare a pointer variable to hold an address of an integer.  Do not worry about 	understanding the exact syntax,  we will cover this in more detail in 	chapter 3.  */
 	 int *integer_pointer;

	  /* declare an integer variable.  This is a container that holds an  integer. */
	  int myint;

	 /* assign an integer to the variable "my_int" */
	  myint = 10;

	 /* print out the address of the integer. This is the unique memory location  where the 	integer is stored. */
	printf("the address of myint is %p.\n",&myint);
  
 	/* Remember that a pointer can store the address of another container.  This means 	that we can put the address of the integer myint into out integer pointer. 	*/
 	 integer_pointer = &myint;

	printf("the integer_pointer holds the address %p.\n",integer_pointer);
}
