#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ppbuf_ut.h"

int main(int argc,char *argv[])
{
   char *source=NULL;
   char **sbuf=NULL;
   char *dot=NULL;

   char newname[80];
   int i;

   if (argc<3)
   {
      fprintf(stderr,"USAGE: qsort -f source_file\n");
      exit(1);
   }
   else
   {
      for (i = 1; i < argc; i+=2)
      {
	 if (argv[i][0] != '-')
	 {
	    fprintf(stderr,"qsort: FATAL - command qualifier must start with a '-'!\n");
	    exit(1);
	 }

	 switch (argv[i][1]) {
	 case 'f':
	    source = argv[i+1];
	    break;
	 default:
	    fprintf(stderr,"qsort: FATAL - command line format incorrect! Retry.\n");
	    exit(1);
	 }
      }
   }

   if ( (sbuf = file2ppbuf(source)) == NULL)
   {
      fprintf(stderr, "qsort: FATAL - unable to buffer %s!\n",source);
      exit(0);
   }

   if (!sortppbuf(sbuf,0))
   {
      fprintf(stderr,"qsort: FATAL - unable to merge sort the file!\n");
      exit(0);
   }

   dot = strrchr(source,'.');
   if (!dot)
	strcpy(newname,"sorted.out");
   else
	*dot = '\0';

   strcpy(newname,source);
   strcat(newname,".srt");
   printf("newname is %s.\n",newname);
   if (!dumpppbuf(sbuf,newname,0,0))
   {
      fprintf(stderr,"qsort: FATAL - unable to dump the buffer to disk!\n");
      exit(0);
   }

   printf("qsort: SUCCESS!\n");
}
