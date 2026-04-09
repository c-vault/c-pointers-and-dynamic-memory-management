/* count.c - this program will read in a block of text (up
to 5000 characters and count the number of characters, words,
and paragraphs in the text.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include "count.h"

/* read text */
int read_text(char *storage)
{
	char inchar;
	int i=0;
	
	while ( ((inchar = getchar()) != EOF) && (i<MAX_CHARS) )
	{
		*storage++ = inchar;
		i++;
	}

	if (i == MAX_CHARS)
		printf("\nMaximum characters (%d) reached!\n",MAX_CHARS);
	
	return(i);
}

/* count */
void count(char *storage, int length, count_posp coords)
{
	int i;
	score thescore= {0,0,0};
	
	short int char_flag=0;
	
	char last_char='~';
	
	gotoxy(coords->char_pos.col,coords->char_pos.row);
	printf("%d",thescore.char_count);
	gotoxy(coords->word_pos.col,coords->word_pos.row);
	printf("%d",thescore.word_count);
	gotoxy(coords->par_pos.col,coords->par_pos.row);
	printf("%d",thescore.par_count);

	for (i=0; i<length; i++)
	{
		if ( (isalnum(storage[i])) || (ispunct(storage[i])) )
		{
			if (!thescore.par_count)
			{
				thescore.par_count++;
				gotoxy(coords->par_pos.col,coords->par_pos.row);
				printf("%d",thescore.par_count);
			}

			if (i == length - 1)
			{
				/* last char is a non-whitespace,
					count last word. */
				thescore.word_count++;
				gotoxy(coords->word_pos.col,coords->word_pos.row);
				printf("%d",thescore.word_count);
			}

			thescore.char_count++;
			gotoxy(coords->char_pos.col,coords->char_pos.row);
			printf("%d",thescore.char_count);
			char_flag = 1;
		} else if ( (isspace(storage[i])) && (char_flag) )
		{
			thescore.word_count++;
			gotoxy(coords->word_pos.col,coords->word_pos.row);
			printf("%d",thescore.word_count);
			char_flag = 0;
		}

		if ( ((storage[i] == '\n') && (last_char == '\n')) ||
		      (storage[i] == '\t')  )
		{
			thescore.par_count++;
			gotoxy(coords->par_pos.col,coords->par_pos.row);
			printf("%d",thescore.par_count);
		}
		last_char = storage[i];
	} /* for */
}

void main()
{
	char text[MAX_CHARS];
	int txt_length = 0;
	int x,y;

	count_pos positions;

	gotoxy(1,1);

	printf("Type text and hit ctrl-D or command-D or ctrl-Z to stop.\n");
	txt_length = read_text(text);

	printf("+------------- COUNT ---------------+\n");
	/* now count the characters */
	y=wherey();

	positions.char_pos.row = y;
	positions.word_pos.row = y+1;
	positions.par_pos.row = y+2;
	positions.char_pos.col = 19;
	positions.word_pos.col = 19;
	positions.par_pos.col = 19;

	printf("# of characters : \n");
	printf("# of words      : \n");
	printf("# of paragraphs : \n");

	count(text,txt_length,&positions);
}