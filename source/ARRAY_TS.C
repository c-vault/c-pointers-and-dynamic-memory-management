#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main()
{
	int int_array[5] = {1,2,3,4,5};
	int i=0;
	
	int *int_ptr;
	
	printf("the address of int_array is %p.\n",int_array);
	printf("the value of the 2nd member is %d.\n",int_array[1]);
	printf("the value of the 3rd member is %d.\n",*(int_array + 2));
	
	int_ptr = int_array;
	
	printf("the value of int_ptr is now %p.\n",int_ptr);
	printf("the value of the 2nd member is %d.\n",*(int_ptr + 1));
	printf("the value of the 3rd member is %d.\n",*(int_ptr + 2));
}