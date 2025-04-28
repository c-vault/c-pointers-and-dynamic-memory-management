/* ppblock_utils.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "ppblock.h"

char *strdup();

ppblockp create_block(char *block_name)
{
	ppblockp outblock;
	
	/* malloc space for the block */
	 outblock = (ppblockp) malloc(sizeof(ppblock));
	 if (!outblock)
	 {
	 	fprintf(stderr,"create_block: FATAL - malloc failed!\n");
	 	return(NULL);
	 }
	 
	 if (!block_name)
	 	outblock->name = NULL;
	 else
	 	outblock->name = strdup(block_name);
	 
	 /* malloc BLOCKSIZE+1 lines for ppbuf */
	 outblock->ppbuf = (char **) malloc(sizeof(char *) * (BLOCKSIZE));
	 if (!outblock->ppbuf)
	 {
	 	fprintf(stderr,"create_block: FATAL - malloc failed!\n");
	 	return(NULL);
	 }
	 
	 outblock->allocated = BLOCKSIZE;
	 outblock->used = 0;
	 
	 return(outblock);
}

int add2block(ppblockp inblock, char *string)
{
	char **temp=NULL;
	
	if (!string || !inblock)
	{
		fprintf(stderr,"add2block: FATAL - parameters may not be NULL!\n");
		return(0);
	}
	
	if (inblock->used >= inblock->allocated)
	{		
		temp = realloc(inblock->ppbuf,
					   sizeof(char *) * (inblock->allocated + BLOCKSIZE));
		if (!temp)
		{
			fprintf(stderr,"add2block: FATAL - unable to add line, realloc failed!\n");
			return(0);
		}
		
		inblock->ppbuf = temp;
		inblock->allocated += BLOCKSIZE;
	}

	inblock->ppbuf[inblock->used] = strdup(string);
	inblock->used++;
	inblock->ppbuf[inblock->used] = NULL;
	return(1);
}

int addbuf2block(ppblockp inblock, char **ppbuf, int length)
{
	char **temp=NULL;
	int i=0,size=0;
	
	if (!ppbuf || !inblock)
	{
		fprintf(stderr,"add2block: FATAL - parameters may not be NULL!\n");
		return(0);
	}
	
	if (!length)
	{
		for (i=0; ppbuf[i]; i++)
			;
		length = i;
	}
	
	if (length>BLOCKSIZE)
		size=length;
	else
		size=BLOCKSIZE;
	
	if ( (inblock->used+length) >= inblock->allocated)
	{		
		temp = realloc(inblock->ppbuf,
					   sizeof(char *) * (inblock->allocated + size));
		if (!temp)
		{
			fprintf(stderr,"add2block: FATAL - unable to add line, realloc failed!\n");
			return(0);
		}
		
		inblock->ppbuf = temp;
		inblock->allocated += size;
	}

	for (i=0; ppbuf[i]; i++)
	{
		inblock->ppbuf[inblock->used] = strdup(ppbuf[i]);
		inblock->used++;
	}
	
	inblock->ppbuf[inblock->used] = NULL;
	return(1);
}