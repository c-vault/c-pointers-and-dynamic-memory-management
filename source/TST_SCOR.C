/* test_scores.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void main()
{
	/* Here is another suggestion, keep all of your same data
	types grouped together. */
	double score_array[100];
	double entered_score=0.0, temp=0.0;
	double high=0.0, low=100.0, average=0.0, sum=0.0;
	int i=0,j=0,good_input=0, bad_input=0;
	int score_cnt=0;

	char user_input[256];
	
	/* explain to the user what the program will do. */
	printf("This program will allow you to enter up to 100 test scores.\n");
	printf("Once scores are entered, I will sort the scores and \n");
	printf("calculate the average, high and low score.\n");
	
	/* we will loop until the user types a -1 or we have 100 scores */
	while ( (entered_score != -1.0) && (score_cnt < 100) )
	{
		good_input = 0; /* set to false */
		/* it is a good practice to error check all user input.
			This will help prevent GIGO! One way to error check
			user input is to allow him to enter characters and
			then check the characters and convert as necessary. */
		while (!good_input)
		{
			bad_input = 0;
			printf("\nEnter a score (-1 to stop): ");
			scanf("%s",user_input);
			/* I will check all the characters in the
			   string for the occurence of any alpha or control char */
			for (i = 0; *(user_input+i); i++)
			{
				if ( (isalpha(*(user_input+i))) || (iscntrl(*(user_input+i))) )
					bad_input = 1;
				if (bad_input) break;
			}
			
			if (!bad_input)
			{
				/* translate the string to a float */
				entered_score = atof(user_input);
				/* error check float */
				if ( ((entered_score>0) && (entered_score<=100)) ||
				      (entered_score == -1) )
				      good_input = 1;
				else
					  good_input = 0;
			}
			else
			{
				/* print an error an try again */
				printf("Incorrect input. Try again (-1 to quit).\n");
				good_input = 0;
			}
		} /* while not good input */
		
		if (entered_score != -1)
		{
			/* now that we have good input, add it to our array. */
			score_array[score_cnt] = entered_score;
			score_cnt++;
		}
				
	} /* while not -1 or a 100 scores entered */
	
	if (score_cnt)
	{
		/* before we sort lets calculate our average, high and low */
		for (i = 0; i < score_cnt; i++)
		{
			sum += *(score_array+i);
			if ( *(score_array+i) > high)
				high = *(score_array+i);				
			if (*(score_array+i) < low)
				low = *(score_array+i);
		}
	
		average = sum/score_cnt;
	
		/* now lets sort the scores using the bubble sort */
		/* bubble sort is the simplest and least efficient sort; however,
		in our non-data intensive example we won't have to worry about
		efficiency. */
		for (i = 1; i < score_cnt; ++i)
		{
			for (j = score_cnt - 1; j >= i; --j)
			{
				if ( *(score_array + j-1) > *(score_array+j) )
				{
					temp = *(score_array + j - 1);
					*(score_array + j - 1) = *(score_array + j);
					*(score_array + j) = temp;
				} /* end if */
			} /* for */
		} /* for */
	
		/* print the results */
		printf("Number of scores entered was %d.\n",score_cnt);
		printf("The sorted list of scores:\n");
		for (i = 0; i < score_cnt; i++)
			printf("\t%f\n",*(score_array + i));
	
		printf("The high score was %f.\n",high);
		printf("The low score was %f.\n",low);
		printf("The average score was %f.\n",average);
	} /* if any scores */
	else
		printf("No scores entered. Bye.\n");
}