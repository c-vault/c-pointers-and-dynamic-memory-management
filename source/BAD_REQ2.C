/* bad_request2.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"

extern char *strdup(char *instr);
extern void memory_map();
extern int check_heap();

void free_ppbuf(char **inbuf, int count)
{
	int i;

	if (inbuf)
	{
		if (count>0)
		{
			for (i = 0; i < count; i++)
				if (inbuf[i]) free(inbuf[i]);
			free(inbuf);
		}
		else
		{
			/* ********* HERE IS WHERE THE PROGRAM WILL CRASH! ****** */
			for (i = 0; inbuf[i]; i++)
				if (inbuf[i]) free(inbuf[i]);
			free(inbuf);
		}
	}
}

char **reverse_ppbuf(char **inbuf)
{
	int count=0,i=0,j=0;
	char **outbuf;

	if (inbuf)
	{
		for (count = 0; inbuf[count]; count++);
		/* ******** HERE IS THE BAD REQUEST ********** */
		outbuf = (char **) malloc(sizeof(char *) * count + 1);
		if (!outbuf)
		{
			fprintf(stderr,"reverse_ppbuf: FATAL - malloc failed!\n");
			return(NULL);
		}

		for (i=0,j=count-1; i < count; i++,j--)
			outbuf[i] = strdup(inbuf[j]);

		printf("reverse_ppbuf: Here is the memory_map before we corrupt the heap!\n");
		memory_map();

		/* null terminate the ppbuf - **** Here we corrupt the heap! */
		outbuf[count] = NULL;

		return(outbuf);
	}
}

void main()
{
	char **mybuf=NULL;
	char **reversed=NULL;

	int i=0;
	
	mybuf = malloc(sizeof(char *) * 4);
	if (!mybuf)
	{
		fprintf(stderr,"bad_request: FATAL - malloc failed!\n");
		exit(0);
	}
	
	mybuf[0] = strdup("one");
	mybuf[1] = strdup("two");
	mybuf[2] = strdup("three");
	mybuf[3] = NULL;
	
	memory_map();
	
	reversed = reverse_ppbuf(mybuf);
	
	if (!check_heap())
	{
		printf("reverse_ppbuf CORRUPTED the HEAP - ABORTING!\n");
		printf("Here is the corrupt heap!\n");
		memory_map();
		exit(0);
	}
	
	for (i = 0; reversed[i]; i++)
		printf("%s\n",reversed[i]);

	free_ppbuf(reversed,0);
}