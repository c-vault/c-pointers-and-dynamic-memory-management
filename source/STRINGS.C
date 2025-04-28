/* strings.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/* #include "memory.h" */

/* strdup will malloc a copy of the input string. */
/* TURBO C has a strdup
char *strdup(char *instr)
{
	char *outstr=NULL;

	if (!instr)
	{
	  fprintf(stderr, "strdup: FATAL - NULL argument!\n");
	  return(NULL);
	}

	outstr = (char *) malloc(sizeof(char) * (strlen(instr) + 1) );
	if (!outstr)
	{
		fprintf(stderr, "strdup: FATAL - malloc failed!\n");
		return(NULL);
	}

	strcpy(outstr, instr);

	return(outstr);
}
*/

/* substr will malloc a substring of a larger string.  If an
error occurs this routine returns NULL.  This routine does null
terminate tho substring.
The substring may be terminated early by a string terminator in
the input string or by a newline character.
NOTE: start is the ith character in the string to start at (for
example if you wanted to start at the first character you would
put 1 and not 0.
numchars is the number of characters to pull out of string1. */
char *substr(char *string1, int start, int numchars)
{
	char *p = NULL;

	int cnt=0, slen=0;

	slen = strlen(string1);
	if ( (slen < 2) || /* is this a valid string? */
	     (start< 1) || /* is start valid? */
	     (start>slen) ) /* is the substring in the string? */
	     return(NULL);

	p = (char *) malloc(sizeof(char) * (numchars + 1));
	if (!p)
	{
		fprintf(stderr,"substr: FATAL - malloc failed!\n");
		return(NULL);
	}

	start--; /* subtract one since C strings start at 0. */
	while (cnt < numchars)
	{
		if ((string1[start+cnt] == '\0') ||
		    (string1[start+cnt] == '\n') ) break;
		p[cnt] = *(string1+start+cnt);
		++cnt;
	} /* end of while */
	p[cnt] = '\0';
	return(p);
}

#define STRAVG 40

char *get_dynamic_str()
{
	char *outstr = NULL;
	char inchar;

	int strmax = STRAVG;
	int charcnt = 0;
	
	/* malloc the outstr */
	/* You should not hard code constants, use a #define */
	outstr = (char *) malloc(sizeof(char) * STRAVG);
	if (!outstr)
	{
		fprintf(stderr,"get_dynamic_str: FATAL - malloc failed on %d bytes.\s", 
				STRAVG);
		return(NULL);
	}
	
	while ( (inchar = getchar()) != '\n')
	{
		if (charcnt == strmax)
		{
			outstr = realloc(outstr,sizeof(char) * strmax + STRAVG);
			if (!outstr)
			{
				fprintf(stderr,"get_dynamic_str: FATAL - realloc failed on %d bytes.\n",
						strmax + STRAVG);
				return(NULL);
			}
			strmax += STRAVG;
		}
		outstr[charcnt] = inchar;
		charcnt++;
	}
	
	/* null-terminate the string */
	outstr[charcnt] = '\0';
	
	return(outstr);
}

#define LINE 40

char *get_file()
{
	char *outstr = NULL;
	char inchar;

	int strmax = LINE;
	int charcnt = 0;
	
	/* malloc the outstr */
	/* You should not hard code constants, use a #define */
	outstr = (char *) malloc(sizeof(char) * LINE);
	if (!outstr)
	{
		fprintf(stderr,"get_dynamic_str: FATAL - malloc failed on %d bytes.\s", 
				STRAVG);
		return(NULL);
	}
	
	while ( (inchar = getchar()) != EOF)
	{
		if (charcnt == strmax)
		{
			outstr = realloc(outstr,sizeof(char) * strmax + LINE);
			if (!outstr)
			{
				fprintf(stderr,"get_dynamic_str: FATAL - realloc failed on %d bytes.\n",
						strmax + LINE);
				return(NULL);
			}
			strmax += LINE;
		}
		outstr[charcnt] = inchar;
		charcnt++;
	}
	
	/* null-terminate the string */
	outstr[charcnt] = '\0';
	
	return(outstr);
}