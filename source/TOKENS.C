/* tokens.c  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MINTOKS 3

/* #define DEBUGTOK */

char *strdup();

char **toks_from_str(char *line,char *delimiters,int *count)
{
   char *temp;
   char **outbuf;

   int numtoks = MINTOKS,cnt=0;
   
   if (!line || !delimiters)
   {
      fprintf(stderr,"toks_from_str: input arguments cannot be NULL!\n");
      *count = 0;
      return(NULL);
   }

   /* malloc space for ppbuf */
   outbuf = (char **) malloc(sizeof(char *) * (MINTOKS+1));
   if (!outbuf)
   {
      fprintf(stderr,"toks_from_str: FATAL - malloc failed!\n");
      *count = 0;
      return(NULL);
   }

#ifdef DEBUGTOK
   printf("malloced space for outbuf.\n");
   printf("count is %d.\n",cnt);
#endif
   
   temp = strdup(line);
   outbuf[cnt] = strtok(temp,delimiters);
   cnt++;

#ifdef DEBUGTOK
   printf("token is %s.\n",outbuf[cnt-1]);
#endif      
   
   while ( (outbuf[cnt] = strtok(NULL,delimiters)) != NULL)
   {
#ifdef DEBUGTOK
      printf("count is %d.\n",cnt);
      printf("token is %s.\n",outbuf[cnt]);
#endif      
      cnt++;
      if (cnt >= numtoks)
      {
	 outbuf = (char **) realloc(outbuf,sizeof(char *) * (numtoks + MINTOKS + 1));
	 if (!outbuf)
	 {
	    fprintf(stderr,"toks_from_str: FATAL - realloc failed!\n");
	    *count = 0;
	    return(NULL);
	 }
	 numtoks += MINTOKS;
      } /* if we need to realloc */
   } /* while strtok does not return NULL */

   *count = cnt;
   return(outbuf);
}
