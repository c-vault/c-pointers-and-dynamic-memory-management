/* ptrptr.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main()
{
	int number=10;
	int *number_ptr=NULL;
	int **number_ptrptr=NULL;
	
	printf("the address of number is %p and it holds %d.\n",
			&number,number);
	number_ptr = &number;
	printf("the address of number_ptr is %p and it holds %p.\n",
			&number_ptr,number_ptr);
	printf("number_ptr points to %d.\n",*number_ptr);
	number_ptrptr = &number_ptr;
	printf("the address of number_ptrptr is %p and it holds %p.\n",
			&number_ptrptr, number_ptrptr);
	printf("number_ptrptr points to %p.\n",*number_ptrptr);
	printf("we can double dereference number_ptrptr to get to %d.\n",
			**number_ptrptr);
}