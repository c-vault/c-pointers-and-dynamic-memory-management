/* BAD_precedence.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MINNUMS 5

#include "memory.h"

extern char *get_dynamic_str();

/* fill array returns the number of entries or 0 on error. */
int fill_array(int **numbers)
{
	char *response=NULL;
	int num=-1,i=0,maxsize=MINNUMS;
	int *temp = NULL;
	
	*numbers = (int *) malloc(sizeof(int) * MINNUMS);
	if (!(*numbers))
	{
		fprintf(stderr,"fill_array: FATAL - malloc failed!\n");
		return(0);
	}
	
	do {
		printf("Enter integer or -1 to stop: ");
		response = get_dynamic_str();
	
		if (response)
			num = atoi(response);
		else
			return(0);
	
		if (num != -1)
		{
			if (i == maxsize)
			{
				/* realloc */
				temp = (int *) realloc(*numbers, 
											sizeof(int) * (maxsize + MINNUMS));				
				if (!temp)
				{
					fprintf(stderr,"fill_array: FATAL - unable to get more space!\n");
					return(i);
				}
				else
					*numbers = temp;
			}
				
			/* fill the array - THIS IS WRONG */
			*numbers[i] = num;
			/**** HERE IS THE CORRECT WAY **** */
			/* (*numbers)[i] = num; */
			i++;
		}
	} while (num != -1);
	
	return(i);
}

void main()
{
	int *my_numbers=NULL;
	int my_count=0,i=0;
	
	my_count = fill_array(&my_numbers);
	
	for	(i=0; i < my_count; i++)
		printf("%d.\n",my_numbers[i]);
}
	
