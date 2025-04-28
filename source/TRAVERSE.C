/* traverse.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* mycopy requires the caller to insure instr is NULL-terminated
and outstr has enough space to hold a copy of instr. */
void mycopy(char *instr, char *outstr)
{
	while ( (*outstr++ = *instr++) != '\0');
}

int slen(char *instr)
{
	int i;
	
	for (i=0; *(instr+i); ++i);
	return(i);
}

/* this is a primitive and slow search program. A much faster
string search for searches of large blocks of text is presented
in the next section on dynamic memory. */
int search(char *searchstr, char *word)
{
	int idx,i,j;
	
	for (i=0; searchstr[i]; i++)
	{
		if (searchstr[i] == word[j])
		{
			if (!j) idx = i;
			j++;
		}
		else
			j = 0;
		
		if (word[j] == '\0')
			break;
	}

	if (word[j] != '\0')
		return(-1);
	else
		return(idx);
}

void reverse(char *instr, char *reverse)
{
	int len,i=0;
	
	len = slen(instr);	
	reverse[len] = '\0'; /* null terminate */	
	while (instr[i] != '\0')
		reverse[--len] = instr[i++];				
}

void main()
{
	char name[80];
	char quote[256];
	char name_copy[80];
	char reverse_name[80];
	
	int index;
	
	/* we will look at four types of string operations:
		copying, counting, searching, reversing. */
	
	/* first let's load some test data into our strings. */
	strcpy(name,"Michael Daconta");
	strcpy(quote,"The C language has brought forth a new era in computing");
	
	/* now let's send name to our own string copy routine */
	printf("name is %s.\n",name);
	mycopy(name,name_copy);
	printf("name_copy is %s.\n",name_copy);
	
	/* let's also write our own string length routine */
	printf("the length of :\n%s\n is %d.\n",quote,slen(quote));
	printf("the length of :\n%s\n is %d.\n",name_copy,slen(name_copy));
	
	/* let's rewrite strstr to return an the index of
	a word in larger string. */	
	index = search(quote, "forth");
	
	if (index != -1)
		printf("the index of \"%s\" in \n%s\n is %d.\n","forth",quote,index);
	else
		printf("%s is not in \n%s\n","forth",quote);
		
	/* now let's write a function to reverse the characters in a string */
	reverse(name,reverse_name);
	printf("my name is %s.\n",name);
	printf("my reverse name is %s.\n",reverse_name);	
}
	
	