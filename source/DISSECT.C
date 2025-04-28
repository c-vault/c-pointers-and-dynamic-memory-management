/* dissect.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern char **file2ppbuf();
extern void free_ppbuf();
extern char *get_dynamic_str();

void main()
{
	char *source=NULL;
	char *dot=NULL;
	char **src_ppbuf=NULL;
	char last_char='~';
	
	int i=0,j=0;
	int words=0, statements=0, blocks=0, comments=0;
	
	short int alnum=0;
		
	printf("Source file to dissect? ");
	source = get_dynamic_str();
	if (!source)
	{
		fprintf(stderr,"dissect: FATAL - get_dynamic_str() failed!\n");
		exit(0);
	}
	
	dot = strrchr(source,'.');
	if (!dot || strcmp(dot,".c"))
	{
		fprintf(stderr,"dissect: FATAL - source file must end in .c!\n");
		exit(0);
	}
	
	src_ppbuf = file2ppbuf(source);
	if (!src_ppbuf)
	{
		fprintf(stderr,"dissect: FATAL - file2ppbuf failed!\n");
		exit(0);
	}
	
	printf("Dissecting ... ");
	fflush(stdout);
	/* go through all the lines in the file */
	for (i=0; src_ppbuf[i]; i++)
	{
		for (j=0; src_ppbuf[i][j]; j++)
		{
			if (isalnum(src_ppbuf[i][j]))
				alnum=1;
			else if (isspace(src_ppbuf[i][j]))
			{
				if (alnum)
				{
					words++;
					alnum=0;
				}
			}
			else if (ispunct(src_ppbuf[i][j]))
			{
				switch (src_ppbuf[i][j]) {
					case ';':
						statements++;
						break;
					case '{':
						blocks++;
						break;
					case '*':
						if (last_char == '/')
							comments++;
						break;
				} /* end of switch */
			} /* end of if */
			last_char = src_ppbuf[i][j];
		} /* end of for all chars on line */
	} /* end of for all lines */

	printf("Done.\n");
	
	printf("+---------- %s ----------+\n",source);	
	printf("Number of words      : %d\n",words);
	printf("Number of statements : %d\n",statements);
	printf("Number of blocks     : %d\n",blocks);
	printf("Number of comments   : %d\n",comments);
	printf("+--------------------------------------+\n");
	
	free_ppbuf(src_ppbuf,0);
			
}