/* debug_memory.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dbg_mem.h"

/* set the memory with header and debug info */
void set_mem(char *ptr, unsigned size,short flag)
{
	headerp hptr;
	unsigned *eptr;

	/* given the pointer to the start of mem, assign the magic numbers
	and the size. */
	hptr = (headerp) ptr;
	hptr->header_magic = flag ? MAGIC_MALLOC : MAGIC_FREE;
	hptr->size = size;
	eptr = (unsigned *) ((char *)ptr + END_OFFSET(size));
	*eptr = flag ? MAGIC_MALLOC : MAGIC_FREE;
}

/* get "raw" system memory */
headerp get_mem(unsigned size)
{
	unsigned tsize=0,bsize=0;

	char *ptr=NULL;

	tsize = TOT_BYTES(size);

	for (bsize = DEFAULT_BLOCK_SIZE; ; bsize >>= 1)
	{
		if (bsize < tsize)
			bsize = tsize;

		if ( (ptr = (char *)sbrk(bsize)) != NULL)
			break;
		else if (bsize == tsize)
			return(NULL);
	}

	/* update the stats */
	stats.current_heap_size += bsize;
	stats.used_blocks++;

	/* set the memory, i.e. give the block structure */
	set_mem(ptr,USABLE_BYTES(bsize),MALLOCFLAG);
	dbg_free(ret_addr(ptr)); /* free the new block to attach it to the free list */
	return(lfree); /* return the block just freed */
}

int check_magic(headerp ptr, short flag)
{
	int head=0,foot=0;

	head = flag? (ptr->header_magic == MAGIC_MALLOC) : (ptr->header_magic == MAGIC_FREE);

	foot = flag? (end_magic(ret_addr(ptr)) == MAGIC_MALLOC) :
						(end_magic(ret_addr(ptr)) == MAGIC_FREE);

	return ((head&&foot));
}

/* check free list traverses the free-list, checks for corruption
and returns the number of blocks in the free-list and the
largest unallocated block. */
int check_free_list(unsigned *largest)
{
	headerp traverse;
	unsigned biggest=0;
	int cnt=0;

	for (traverse = freelist.next; !(traverse == &freelist); traverse = traverse->next)
	{
		if (!check_magic(traverse,FREEFLAG))
		{
			printf("check_free_list: BAD MAGIC NUMBER! CORRUPTED HEAP! ABORT!\n");
			exit(0);
		}

		cnt++;
		if (traverse->size > biggest)
			biggest = traverse->size;
	}

	*largest = biggest;
	return(cnt);
}

/* check free list traverses the free-list, checks for corruption
and returns the number of blocks in the free-list and the
largest unallocated block. */
void print_free_list(void)
{
	headerp traverse;
	int cnt=0;

	printf("FREELIST\n");
	for (traverse = freelist.next; !(traverse == &freelist); traverse = traverse->next)
	{
		if (!check_magic(traverse,FREEFLAG))
		{
			printf("check_free_list: BAD MAGIC NUMBER! CORRUPTED HEAP! ABORT!\n");
			exit(0);
		}

		cnt++;
		printf("block[%d]->size(%d) ",cnt,traverse->size);
		if (!(cnt % 3)) printf("\n");
	}
	printf("\n");
}

void memory_map(void)
{
	headerp traverse,inbetween;
	int cnt=0, block_cnt=stats.used_blocks;

	printf("\n");
	for (traverse = freelist.next; !(traverse == &freelist); traverse = traverse->next)
	{
		cnt++;
		printf("[%d]%p: FREE (%d) ",cnt,ret_addr(traverse),traverse->size);
		if (!(cnt % 3))
			printf("\n");
		else
			fflush(stdout);

		inbetween = (headerp) ( (char *) traverse + TOT_BYTES(traverse->size));
		if (inbetween != traverse->next)
		{
		  if (traverse->next != &freelist)
		  {
			while (inbetween < traverse->next)
			{
				/* if we hit a non-malloced block, which could occur if
				there is only one free block, and when the heap is not
				contiguous. get out and go to the next free block. (if any) */
				if (inbetween->header_magic != MAGIC_MALLOC)
					break;
				else
				{
					if (end_magic(ret_addr(inbetween)) != MAGIC_MALLOC)
					{
						printf("memory_map: CORRUPTED HEAP! ABORT!\n");
						exit(0);
					}
				}

				cnt++;
				printf("[%d]%p:MALLOC(%d) ",cnt,ret_addr(inbetween),inbetween->size);
				if (!(cnt % 3))
					printf("\n");
				else
					fflush(stdout);

				/* decrement # of malloced blocks printed out. */
				block_cnt--;
				inbetween = (headerp) ( (char *) inbetween + TOT_BYTES(inbetween->size));
			} /* end of while */
		  } /* if traverse->next != &freelist */
		  else /* traverse->next does equal &freelist */
		  {
			/* find out how many malloced blocks are left
			   to be printed out. */
			while (block_cnt)
			{
				/* if we hit a non-malloced block, which could occur if
				there is only one free block, and when the heap is not
				contiguous. get out and go to the next free block. (if any) */
				if (inbetween->header_magic != MAGIC_MALLOC)
					break;
				else
				{
					if (end_magic(ret_addr(inbetween)) != MAGIC_MALLOC)
					{
						printf("memory_map: CORRUPTED HEAP! ABORT!\n");
						exit(0);
					}
				}

				cnt++;
				printf("[%d]%p:MALLOC(%d) ",cnt,ret_addr(inbetween),inbetween->size);
				if (!(cnt % 3))
					printf("\n");
				else
					fflush(stdout);

				/* decrement # of malloced blocks printed out. */
				block_cnt--;
				inbetween = (headerp) ( (char *) inbetween + TOT_BYTES(inbetween->size));
			} /* while block_cnt */
		  } /* if traverse->next == &freelist */
		} /* inbetween != traverse->next */

	 } /* end of for */
	 printf("\n");
}

int check_heap(void)
{
	headerp traverse,inbetween;
	int block_cnt=stats.used_blocks;

	for (traverse = freelist.next; !(traverse == &freelist); traverse = traverse->next)
	{

		if (!check_magic(traverse,FREEFLAG))
			return(0);

		inbetween = (headerp) ( (char *) traverse + TOT_BYTES(traverse->size));
		if (inbetween != traverse->next)
		{
		  if (traverse->next != &freelist)
		  {
			while (inbetween < traverse->next)
			{
				if (inbetween->header_magic != MAGIC_MALLOC)
					break;
				else
				{
					if (end_magic(ret_addr(inbetween)) != MAGIC_MALLOC)
						return(0);
				}
	 			
				/* decrement # of malloced blocks printed out. */
				block_cnt--;
				inbetween = (headerp) ( (char *) inbetween + TOT_BYTES(inbetween->size));
	 		} /* end of while */
		  } /* if traverse->next != &freelist */
		  else /* traverse->next does equal &freelist */
		  {
			/* find out how many malloced blocks are left
			   to be printed out. */
			while (block_cnt)
			{
				/* if we hit a non-malloced block, which could occur if
				there is only one free block, and when the heap is not
				contiguous. get out and go to the next free block. (if any) */
				if (inbetween->header_magic != MAGIC_MALLOC)
					break;
				else
				{
					if (end_magic(ret_addr(inbetween)) != MAGIC_MALLOC)
					{
						return(0);
					}
				}

				/* decrement # of malloced blocks printed out. */
				block_cnt--;
				inbetween = (headerp) ( (char *) inbetween + TOT_BYTES(inbetween->size));
			} /* while block_cnt */
		  } /* if traverse->next == &freelist */
		}
	 } /* end of for */
	 
	return(1);
}			
	
void *dbg_malloc(unsigned num_bytes)
{
	headerp traverse, previous_ptr;
	
	if (num_bytes == 0)
	{
		printf("dbg_malloc: FATAL - num_bytes is 0!\n");
		exit(0);
	}
	
	/* check if num_bytes is outlandish */	
	if (num_bytes > DEBUG_BOUND)
	{
		printf("dbg_malloc: FATAL - requested %ld bytes : %ld over upper bound of %ld.\n",
				num_bytes, DEBUG_BOUND - num_bytes, DEBUG_BOUND);
		exit(0);
	}
	
	if ( (previous_ptr = lfree) == NULL)
	{
		freelist.next = previous_ptr = lfree = &freelist;
		freelist.size = 0;
	}	
		
	/* search for a memory block big enough. */
	for (traverse = previous_ptr->next; ; previous_ptr = traverse, 
		 traverse = traverse->next)
	{
		if (traverse != &freelist)
		{
			if (!check_magic(traverse,FREEFLAG))
			{
				printf("dbg_malloc: BAD MAGIC NUMBER! CORRUPTED HEAP! ABORT!\n");
				exit(0);
			} 
		}
		
		/* traverse->size is a count of the USABLE BYTES */
		if ( (traverse->size == MULTIPLE_OF(num_bytes)) ||
			 (traverse->size >= TOT_BYTES(num_bytes)) ) /* looking for first memory equal to or bigger.*/
		{
			if (traverse->size == MULTIPLE_OF(num_bytes)) /* exact match */
			{
				previous_ptr->next = traverse->next; /* delink from free list */
				set_mem((char *)traverse,traverse->size,MALLOCFLAG);
			}
			else
			{
				/* reduce size of current block so we can strip off the end. */
				traverse->size -= TOT_BYTES(num_bytes);
				/* set the memory of the reduced free block */
				set_mem((char *)traverse,traverse->size,FREEFLAG);
				traverse = (headerp) ( (char *) traverse + TOT_BYTES(traverse->size));
				/* set the memory of the tail portion. */
				set_mem((char *)traverse, MULTIPLE_OF(num_bytes),MALLOCFLAG);
			}
			
			lfree = previous_ptr; /* set previous ptr to the place before the
									just-malloced memory so that if that memory
									gets freed right away, we will be ready at
									that spot. */
			
			/* update the stats */
			stats.used_blocks++;
			
			return(ret_addr(traverse));
			
		} /* end of if */
				
		if (traverse == lfree)
			if ( (traverse = get_mem(num_bytes)) == NULL)
				return(NULL);
		
	} /* end of for */
}

void dbg_free(void *ptr)
{
	headerp block_header=NULL,traverse;

	if (!ptr)
	{
		printf("dbg_free: Attempt to free a NULL pointer!\n");
		return;
	}
	else
		block_header = (headerp) ((char *)ptr - START_BYTES);
		
	/****** *check to make sure the pointer passed in has
	a MALLOC_MAGIC ***** */
		
	if (block_header->header_magic != MAGIC_MALLOC)
	{
		if (block_header->header_magic == MAGIC_FREE)
		{
			printf("dbg_free: Trying to free a freed pointer!\n");
			return;
		}
		else
		{	
			printf("dbg_free: Trying to free a NON-MALLOCED pointer!\n");
			return;
		}
	}
	
	/* update the stats */
	stats.used_blocks--;
	
	/* find where this block fits in the list. Should be inbetween
	two elements in the list. */
	for (traverse = lfree; 	
	!(block_header > traverse && block_header < traverse->next); 
	traverse = traverse->next)
	{
		/* if traverse is greater then its next pointer, we are at the end
		of the list. AND block header must be either greater then the end
		or before the beginning. */
		if (traverse >= traverse->next && (block_header > traverse ||
											block_header < traverse->next))
											break;
	}
			
	/* if the end_of_this_block equals the start of the next, connect to upper */
	if (((char *) block_header + TOT_BYTES(block_header->size)) == 
									(char *) traverse->next)
	{
		block_header->size += TOT_BYTES(traverse->next->size);
		block_header->next = traverse->next->next;
		set_mem((char *)block_header,block_header->size,FREEFLAG);
	}
	else
	{
		set_mem((char *)block_header,block_header->size,FREEFLAG);
		block_header->next = traverse->next;
	}
	
	/* If the end of the previous block equals this blocks address,
	connect to lower */
	if (((char *) traverse + TOT_BYTES(traverse->size)) == 
										(char *) block_header)
	{
		traverse->size += TOT_BYTES(block_header->size);
		traverse->next = block_header->next;
		set_mem((char *)traverse,traverse->size,FREEFLAG);
	}
	else
		traverse->next = block_header;
	
	lfree = traverse;
}

/* strdup will malloc a copy of the input string. */
char *dbg_strdup(char *instr)
{
	char *outstr=NULL;
	
	if (!instr)
	{
	  fprintf(stderr, "strdup: FATAL - NULL argument!\n");
	  return(NULL);
	}
	 
	outstr = (char *) dbg_malloc(sizeof(char) * (strlen(instr) + 1) );
	if (!outstr)
	{
		fprintf(stderr, "strdup: FATAL - malloc failed!\n");
		return(NULL);
	}
	
	strcpy(outstr, instr);
	
	return(outstr);
}

void print_stats(void)
{
	stats.free_blocks = check_free_list(&(stats.largest_free_block));
	stats.total_free_mem = (unsigned long) coreleft();
	
	printf("HEAP STATISTICS\n");
	printf(" - current heap size      : %ld\n",stats.current_heap_size);
	printf(" - free blocks            : %d\n",stats.free_blocks);
	printf(" - used blocks            : %d\n",stats.used_blocks);
	printf(" - largest free block     : %d\n",stats.largest_free_block);
	printf(" - system memory left     : %ld\n",stats.total_free_mem);
}	

void *dbg_realloc(void *ptr, long size)
{
	headerp block_header,freeblock;
	void *newp=NULL;
	
	if (!ptr)
		return(dbg_malloc(size));
		
	block_header = (headerp) ((char *)ptr - START_BYTES);
	if (block_header->size < MULTIPLE_OF(size))
	{
		/* get a larger block */
		newp = dbg_malloc(size);
		
		if (!newp)
			return(NULL);
		memcpy(newp,ptr,block_header->size);
		
		dbg_free(ptr);
		return(newp);
	}
	else if (block_header->size < TOT_BYTES(size))
		return(ptr);
	else
	{
		/* chop off the end of the block */
		freeblock = (headerp) ((char *) block_header + TOT_BYTES(size));
		set_mem((char *)freeblock, block_header->size - TOT_BYTES(size),MALLOCFLAG);
		dbg_free(ret_addr(freeblock));
		set_mem((char *)block_header,MULTIPLE_OF(size),MALLOCFLAG);
		return(ptr);
	}
}