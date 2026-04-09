/* this will be a quick and dirty implementation of a stack using a char ptr ptr buffer. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "str_stck.h"

extern char *strdup();

ppstackp init_ppstack()
{
   ppstackp outstack;

   /* malloc space for the stack structure */
   outstack = (ppstackp) malloc(sizeof(ppstack));
   if (!outstack)
   {
      fprintf(stderr,"init_stack: FATAL - malloc failed!\n");
      return(NULL);
   }

   /* fill in the initial values of this stack */
   outstack->queue = (char **) malloc(sizeof(char *) * (STACK_GROW + 1));
   if (!outstack->queue)
   {
      fprintf(stderr,"init_stack: FATAL - malloc failed!\n");
      if (outstack) free(outstack);
      return(NULL);
   }

   outstack->top = -1;
   outstack->stack_qty = 0;
   outstack->stack_max = STACK_GROW;

   return(outstack);
}

/* multiple push. Null terminate end of the list. */
int mpush(ppstackp thestack,...)
{
   char **temp;
   char *next,*data;

   va_list args;
   
   if (!thestack)
   {
      fprintf(stderr,"mpush: FATAL - stack pointer is NULL!\n");
      return(0);
   }

   va_start(args, thestack);
   
   while (next = va_arg(args,char *))
   {
      data = strdup(next);
      /* make a copy of the data string and push it onto the stack */
      /* first check if there is enough room on the stack */
      if ( (thestack->stack_qty + 1) >= thestack->stack_max)
      {
	 /* grow the stack before adding, realloc in chunks of STACK_GROW */
	 temp = (char **) realloc(thestack->queue,
				  sizeof(char *) * (thestack->stack_max + STACK_GROW));
	 if (!temp)
	 {
	    fprintf(stderr,"mpush: FATAL - unable to GROW STACK!\n");
	    return(0);
	 }

	 thestack->queue = temp;
	 thestack->stack_max += STACK_GROW;
      }

      /* increment top pointer now that we know the stack has enough room */
      thestack->top++;
   
      /* malloc the space for this new element */
      thestack->queue[thestack->top] = (char *) malloc(sizeof(char) * (strlen(data) + 1));
      if (!thestack->queue[thestack->top])
      {
	 fprintf(stderr,"mpush: FATAL - unable to store data, malloc failed!\n");
	 return(0);
      }

      /* copy the data */
      strcpy(thestack->queue[thestack->top],data);

      /* increment stack quantity */
      thestack->stack_qty++;
      if (data) free(data);
   }
   
   return(1);   
}

int pushstr(ppstackp thestack, char *data)
{
   char **temp;
   
   if (!data)
   {
      fprintf(stderr,"pushtr: FATAL - data argument is NULL!\n");
      return(0);
   }

   if (!thestack)
   {
      fprintf(stderr,"pushtr: FATAL - stack pointer is NULL!\n");
      return(0);
   }

   /* make a copy of the data string and push it onto the stack */
   /* first check if there is enough room on the stack */
   if ( (thestack->stack_qty + 1) >= thestack->stack_max)
   {
      /* grow the stack before adding, realloc in chunks of STACK_GROW */
      temp = (char **) realloc(thestack->queue,
			       sizeof(char *) * (thestack->stack_max + STACK_GROW));
      if (!temp)
      {
	 fprintf(stderr,"pushstr: FATAL - unable to GROW STACK!\n");
	 return(0);
      }

      thestack->queue = temp;
      thestack->stack_max += STACK_GROW;
   }

   /* increment top pointer now that we know the stack has enough room */
   thestack->top++;
   
   /* malloc the space for this new element */
   thestack->queue[thestack->top] = (char *) malloc(sizeof(char) * (strlen(data) + 1));
   if (!thestack->queue[thestack->top])
   {
      fprintf(stderr,"pushstr: FATAL - unable to store data, malloc failed!\n");
      return(0);
   }

   /* copy the data */
   strcpy(thestack->queue[thestack->top],data);

   /* increment stack quantity */
   thestack->stack_qty++;

   return(1);   
}

char **mpop(ppstackp thestack,int number_topop)
{
   char **data;
   int i=0;
   
   /* check if the stack pointer is valid */
   if (!thestack || !number_topop)
   {
      fprintf(stderr,"mpop: FATAL - invalid input arpuments!\n");
      return(NULL);
   }   

   /* check if there is anything on the stack */
   if ( (thestack->top<0) || (thestack->stack_qty<1) )
   {
      fprintf(stderr,"mpop: FATAL - you cannot pop an empty stack!\n");
      return(NULL);
   }

   /* check if there are enough items to pop */
   if ( thestack->stack_qty < number_topop )
   {
      fprintf(stderr,"mpop: FATAL - you requested %d items, when there are only %d!\n",
	      number_topop,thestack->stack_qty);
      return(NULL);
   }
   
   /* initialize the output buffer */
   data = (char **) malloc(sizeof(char *) * (number_topop + 1));
   if (!data)
   {
      fprintf(stderr,"mpop: FATAL - malloc failed!\n");
      return(NULL);
   }
   
   for (i = 0; i < number_topop; i++)
   {
      /* give the user a copy of the data then free it */
      data[i] = strdup(thestack->queue[thestack->top]);
      
      /* free the top of the stack */
      if (thestack->queue[thestack->top]) free(thestack->queue[thestack->top]);
      
      /* decrement the top and the stack quantity */
      thestack->top--;
      thestack->stack_qty--;
   }

   data[number_topop] = NULL;
   return(data);
}

char *popstr(ppstackp thestack)
{
   char *data;
   
   /* check if the stack pointer is valid */
   if (!thestack)
   {
      fprintf(stderr,"popstr: FATAL - stack pointer is NULL!\n");
      return(NULL);
   }   

   /* check if there is anything on the stack */
   if ( (thestack->top<0) || (thestack->stack_qty<1) )
   {
      fprintf(stderr,"popstr: FATAL - you cannot pop an empty stack!\n");
      return(NULL);
   }

   /* give the user a copy of the data then free it */
   data = strdup(thestack->queue[thestack->top]);

   /* free the top of the stack */
   if (thestack->queue[thestack->top]) free(thestack->queue[thestack->top]);

   /* decrement the top and the stack quantity */
   thestack->top--;
   thestack->stack_qty--;
   
   return(data);
}

void print_ppstack(ppstackp thestack)
{
   int i;

   /* check if the stack pointer is valid */
   if (!thestack)
   {
      fprintf(stderr,"print_ppstack: FATAL - stack pointer is NULL!\n");
      return;
   }   

   /* check if there is anything on the stack */
   if ( (thestack->top<0) || (thestack->stack_qty<1) )
   {
      fprintf(stderr,"print_ppstack: FATAL - you cannot print an empty stack!\n");
      return;
   }

   printf("_____< TOP OF STACK >_____\n");
   for (i = thestack->top; i>=0; i--)
      printf("\t%s\n",thestack->queue[i]);
   printf("__________________________\n");         
}

void free_ppstack(ppstackp thestack)
{
   int i;

   /* check if the stack pointer is valid */
   if (!thestack)
   {
      fprintf(stderr,"free_ppstack: FATAL - stack pointer is NULL!\n");
      return;
   }   

   /* check if there is anything on the stack */
   if ( (thestack->top<0) || (thestack->stack_qty<1) )
   {
      fprintf(stderr,"the_ppstack: FATAL - you cannot free an empty stack!\n");
      return;
   }

   for (i = thestack->top; i>=0; i--)
      if (thestack->queue[i]) free(thestack->queue[i]);
   if (thestack->queue) free(thestack->queue);
   if (thestack) free(thestack);
}
      


