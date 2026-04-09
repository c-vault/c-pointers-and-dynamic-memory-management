/* pass_address.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
void main()
{
	int shella=10, shellb = 20;
	printf("Before swap, shella holds %d and shellb holds %d.\n",
		shella, shellb);
	swap(&shella, &shellb);
	printf("After swap, shella holds %d and shellb holds %d.\n",
		shella, shellb);
}