#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "gll.h"
/* =========================================================================
   Here are the Generic Linked List routines.
   Programmer: MCD - Mar 92
   ========================================================================= */

glptr initgll(int numstr,int numint,int numflt,...)
{
   glptr head;
   int i,j,k;
   va_list args;
   char *shold;
   int ihold;
   float fhold;
   
   if ( (numstr<0) || (numint<0) || (numflt<0) )
   {
      fprintf(stderr,"initgll: parameter count less than 0.\n");
      return(NULL);
   }
   else
      head = (glptr) malloc(sizeof(struct genericlist));
      
    if (!head)
    {
       fprintf(stderr,"initgll: error mallocing structure memory.\n");
       return(NULL);
    }
   
   va_start(args,numflt);
   
   /* store the counts in the structure. */
   head->ns = numstr; head->ni = numint; head->nf = numflt;
   
   /* malloc the memory for the structure, based upon the counts.
      After the memory for the structure is malloced. Loop
      through the field count, malloc the memory for the
      fields and assign the fields. */
   if (numstr>0)
   {
      head->Strings = (char **) malloc(sizeof(char *) * numstr);
      if (!head->Strings)
      {
	   fprintf(stderr,"initgll: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (i = 0; i < numstr; i++)
      {
 		 shold = va_arg (args,char *);
		 head->Strings[i] = (char *)malloc(sizeof(char) * (strlen(shold) + 1));
		 strcpy(head->Strings[i], shold);
      }
   }
   
   if (numint>0)
   {
      head->Integers = (int *) malloc(sizeof(int) * numint);
      if (!head->Integers)
      {
	   fprintf(stderr,"initgll: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (j = 0; j < numint; j++)
      {
	   ihold = (int) va_arg(args,double);
	   head->Integers[j] = ihold;
      }
   }
      
   if (numflt>0)
   {
      head->Floats = (float *) malloc(sizeof(float) * numflt);
      if (!head->Floats)
      {
	   fprintf(stderr,"initgll: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (k = 0; k < numflt; k++)
      {
  	   fhold = (float) va_arg(args,double);
	   head->Floats[k] = fhold;
      }
   }
   
   va_end(args);
   
   /* now assign the next pointer to null */

   head->next = NULL;

   /* now return the head pointer */

   return(head);

}
   

int addgll_node(glptr start, ...)
{
   glptr node;
   glptr traverse;
   int i,j,k;
   va_list args;
   char *shold;
   int ihold;
   float fhold;
   int nums,numi,numf;
   
   if (!start)
   {
      fprintf(stderr,
	      "\a\aaddgll_node: list pointer is NULL.\n");
      return(0);
   }

   /* retrieve the field counts from the head node */
   nums = start->ns;
   numi = start->ni;
   numf = start->nf;

   /* traverse to the end of the list */
   for (traverse = start; traverse->next; traverse = traverse->next)
      ;

   /* malloc space for the new node */
   node = (glptr) malloc(sizeof(struct genericlist));
   if (!node)
   {
      fprintf(stderr,"addgll_node: error mallocing structure memory.\n");
      return(0);
   }
   
   va_start(args,start);
   
   /* store the counts in the structure. */
   node->ns = nums; node->ni = numi; node->nf = numf;
   
   /* malloc the memory for the structure, based upon the counts.
      After the memory for the structure is malloced. Loop
      through the field count, malloc the memory for the
      fields and assign the fields. */
   if (nums>0)
   {
      node->Strings = (char **) malloc(sizeof(char *) * nums);
      if (!node->Strings)
      {
	    fprintf(stderr,"addgll_node: error mallocing structure memory.\n");
	    return(0);
      }
      
      for (i = 0; i < nums; i++)
      {
	   shold = va_arg(args,char *);
	   node->Strings[i] = (char *) malloc(sizeof(char) * (strlen(shold) + 1));
	   strcpy(node->Strings[i], shold);
      }
   }
   
   if (numi>0)
   {
      node->Integers = (int *) malloc(sizeof(int) * numi);
      if (!node->Integers)
      {
	 	fprintf(stderr,"addgll_node: error mallocing structure memory.\n");
	 	return(0);
      }
      
      for (j = 0; j < numi; j++)
      {
	 	ihold = (int) va_arg(args,double);
	 	node->Integers[j] = ihold;
      }
   }
         
   if (numf>0)
   {
      node->Floats = (float *) malloc(sizeof(float) * numf);
      if (!node->Floats)
      {
	 	fprintf(stderr,"addgll_node: error mallocing structure memory.\n");
	 	return(0);
      }
      
      for (k = 0; k < numf; k++)
      {
	 	fhold = (float) va_arg(args,double);
	 	node->Floats[k] = fhold;
      }
   }

   va_end(args);
   
   /* now assign the next pointer to null */

   node->next = NULL;

   /* now connect this new node to the list */

   traverse->next = node;

   /* return success */
   return(1);
}

      
void printgll(glptr start,FILE *fptr)
{
   glptr traverse;
   int ns, ni, nf;
   int i,j,k,cnt;
   
   /* retrieve the number of fields from the start node */
   ns = start->ns;
   ni = start->ni;
   nf = start->nf;
   
   for (traverse = start,cnt=1; traverse; traverse = traverse->next,cnt++)
   {
      fprintf(fptr,"--------------- node %d ------------------\n",cnt);
      for (i = 0; i < ns; i++)
	    fprintf(fptr,"%s\n",traverse->Strings[i]);
      for (j = 0; j < ni; j++)
	    fprintf(fptr,"%d,",traverse->Integers[j]);
      fprintf(fptr,"\n");
      for (k = 0; k < nf; k++)
	    fprintf(fptr,"%f,",traverse->Floats[k]);
      fprintf(fptr,"\n");
   }
   fprintf(fptr,"------------------------------------------\n");
}

glptr srchgll(glptr start, int numkeys, ...)
{
   struct key{
      char keytype;
      int keypos;
      void *keyval; /* a typeless key value */
   };
   struct key **keys;
   va_list args;
   char *shold;
   int i,ihold;
   float fhold;
   glptr traverse;
   int keysmatch=0;
   
   if (!start)
   {
      fprintf(stderr,
	      "\a\asrchgll: linked list pointer is NULL.\n");
      return(NULL);
   }

   if ((!numkeys) || (numkeys < 0))
   {
      fprintf(stderr,
	      "\a\asrchgll: number of keys cannot be 0 or negative.\n");
      return(NULL);
   }
   
   /* malloc the space for numkeys */

   keys = (struct key **) malloc(sizeof(struct key *) * numkeys);

   if (!keys)
   {
      fprintf(stderr,
	      "\a\asrchgll: mallocing error.\n");
      return(NULL);
   }

   va_start(args, numkeys);

   /* loop through and retrieve key values */
   for (i = 0; i < numkeys; i++)
   {
      keys[i] = (struct key *) malloc(sizeof(struct key));
      keys[i]->keytype = (char) va_arg(args, int);
      keys[i]->keypos = (int) va_arg(args,double);
      switch (keys[i]->keytype) {
       case 's':
	    shold = (char *) va_arg(args,char *);
	    keys[i]->keyval = (char *) malloc(sizeof(char) * (strlen(shold) + 1));
	    strcpy(keys[i]->keyval, shold);
	   break;
       case 'i':
	    keys[i]->keyval = (int *) malloc(sizeof(int));
	    ihold = (int) va_arg(args,double);
	    memcpy(keys[i]->keyval,&ihold,sizeof(int));
	   break;
       case 'f':
	    keys[i]->keyval = (float *) malloc(sizeof(float));
	    fhold = (float) va_arg(args,double);
	    memcpy(keys[i]->keyval,&fhold,sizeof(float));
	    break;
       default:
	    fprintf(stderr,
		 "\a\asrchgll: key case is incorrect.\n");
	   return(NULL);
      }
   }

   /* now we can traverse the list and check if any of the
      nodes match all of the keys.  The node must match ALL of
      the specified key values exactly! */

   for (traverse = start; traverse; traverse=traverse->next)
   {
      keysmatch = 0;
      for (i = 0; i < numkeys; i++)
      {
	    switch (keys[i]->keytype) {
	     case 's':
	      if (!(strcmp(traverse->Strings[(keys[i]->keypos)-1],
		   keys[i]->keyval)))
	         keysmatch++;
	      break;
	     case 'i':
	      memcpy(&ihold,keys[i]->keyval,sizeof(int));
	      if ( (traverse->Integers[(keys[i]->keypos)-1]) == ihold)
	         keysmatch++;
	      break;
	     case 'f':
	      memcpy(&fhold,keys[i]->keyval,sizeof(float));
	     if ( (traverse->Floats[(keys[i]->keypos)-1]) == fhold)
		 keysmatch++;
	      break;
	   }
	   if(keysmatch < (i + 1)) break;
      }
      if (keysmatch == numkeys)
      {
	for (i = 0; i < numkeys; i++)
	 if (keys[i]) free(keys[i]);
	if (keys) free(keys);
	 return(traverse);
      }
   }

   for (i = 0; i < numkeys; i++)
     if (keys[i]) free(keys[i]);
   if (keys) free(keys);
   return(NULL);
}

glptr delgll_node(glptr start, glptr searchptr)
{
   glptr traverse;
   glptr temp;
   int i,j,k;

   if ( (!start) || (!searchptr) )
   {
      fprintf(stderr,
	      "\a\adelgll_node: argument pointer set to NULL\n");
      return(NULL);
   }

   if (start == searchptr)
   {
      temp = start->next;
      /* free the heap space used */
	  if (start->ns > 0)
	  {
        for (i = 0; i< start->ns; i++)
	      if (start->Strings[i]) free(start->Strings[i]);
        if (searchptr->Strings) free (searchptr->Strings); 
      }
	  if (start->ni > 0)
	  {
        if (searchptr->Integers) free (searchptr->Integers); 
      }
	  if (start->nf > 0)
	  {
       if (searchptr->Floats) free (searchptr->Floats); 
      }
      free(start); 
      start = temp;
      return(start);
   }      
   
   for (traverse=start; traverse; traverse=traverse->next)
   {
      if (traverse->next == searchptr)
      {
	   traverse->next = searchptr->next;
	   /* free the heap space used */
	   if (start->ns > 0)
	   {
	     for (i = 0; i< start->ns; i++)
	      if (searchptr->Strings[i]) free(searchptr->Strings[i]);
	     if (searchptr->Strings) free(searchptr->Strings); 
       }
	   if (start->ni > 0)
	   {
	     if (searchptr->Integers) free(searchptr->Integers);
       }
	   if (start->nf > 0)
	   {
	     if (searchptr->Floats) free(searchptr->Floats);
       }
	   free(searchptr);
	   return(start);
      }
   }
   return(NULL);
}

int extractgll_node(glptr start, glptr searchptr, ...)
{
   glptr traverse;
   va_list args;
   int numstr,numint,numflt;
   char *sptr;
   int *iptr;
   float *fptr;
   int i,j,k;
   if ( (!start) || (!searchptr) )
   {
      fprintf(stderr,
	      "\a\aextractgll_node: argument pointer set to NULL\n");
      return(0);
   }

   for (traverse=start; traverse; traverse=traverse->next)
   {
      if (traverse == searchptr)
      {
	 va_start(args,searchptr);
   
	 /* store the counts */
	 numstr = start->ns; numint=start->ni; numflt=start->nf;
   
	 if (numstr>0)
	 {
	    for (i = 0; i < numstr; i++)
	    {
  
	       sptr = va_arg (args, char *);
	       strcpy(sptr,traverse->Strings[i]);   
	    }
	 }
   
	 if (numint>0)
	 {   
	    for (j = 0; j < numint; j++)
	    {
	       iptr = va_arg(args,int *);
	       memcpy(iptr,&(traverse->Integers[j]), sizeof(int));
	    }
	 }
      
	 if (numflt>0)
	 {

#ifdef DEBUG_EXTRACTGLL_NODE
	    printf("extractgll_node:  Extracting the floats.\n");
#endif
   
	    for (k = 0; k < numflt; k++)
	    {
	       fptr = va_arg(args,float *);
	       memcpy(fptr,&(traverse->Floats[k]), sizeof(float));
	    }
	 }

	 va_end(args);

	 return(1);
      } /* if */
   } /* for */
   return(0);
}

void delgll(glptr start)
{
   int i,j,k;
   glptr traverse = 0L;
   glptr temp = 0L;

   if(!start)
   {
      fprintf(stderr,
	      "delgll:  Cannot free a NULL linked list.\n");
      return;
   }
   
   for(traverse = start;traverse->next;traverse = temp)
   {

      temp = traverse->next;
      
      /* free the heap space used */

      for (i = 0; i < traverse->ns; i++)
	 	free(traverse->Strings[i]);

      if(traverse->ns > 0) free(traverse->Strings);  
      if(traverse->ni > 0) free(traverse->Integers);
      if(traverse->nf > 0) free(traverse->Floats);

      free(traverse);

   }
}

