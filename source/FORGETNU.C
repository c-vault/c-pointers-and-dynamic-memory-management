/* forget_null.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char *strip_trailing_blanks(char *instr)
{
	int len=0,i=0,j=0;
	char *outstr;
	
	if (instr)
	{
		len = strlen(instr);
		
		for (i = len - 1; ((instr[i] == ' ') && (i >= 0)); i--);
		
		i++;
		outstr = malloc(sizeof(char) * (i+1));
		if (!outstr)
		{
			fprintf(stderr,"strip_trailing_blanks: FATAL - malloc failed!\n");
			return(NULL);
		}
		
		printf("last blank is %d.\n",i);
		
		for (j = 0; j < i; j++)
			outstr[j] = instr[j];
			
		/* *** here is where we are forgetting something *** */

	}
	return(outstr);
}

void main()
{
	char myname[] = "Jack Waste Space          ";
	char *stripped = NULL;
	int mlen = 0, slen = 0;

	mlen = strlen(myname);
	printf("length of myname is %d.\n",mlen);
	stripped = strip_trailing_blanks(myname);
	slen = strlen(stripped);
	printf("stripped is <%s> with length %d.\n",stripped,slen);
}
	
	