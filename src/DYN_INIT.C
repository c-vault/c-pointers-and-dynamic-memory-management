/* dynamic_initialize.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TESTNUM 5

extern char *get_dynamic_str();

/* It is useful to use the return of the
   function for an error status and at the same
   time have the function get data for the main
   program. It is just this case where we first
   encounter "pointer pointers" */

int get_data(char **date, char **item, float *price)
{
	char cdate[80];
	
	/* let's look at what is on the stack */
	printf("date is %p.\n",date);
	printf("item is %p.\n",item);
	printf("price is %p.\n",price);	
	
	/* Getting the first string in this fashion is
	only used to illustrate filling the dynamic character
	array manually with malloc. We really should use
	get_dynamic_str because it is easier. */
	printf("Enter the date: ");
	scanf("%s",cdate);
	fflush(stdin);
	
	*date = (char *) malloc(sizeof(char) * (strlen(cdate) + 1));
	if (!*date)
	{
		fprintf(stderr,"get_data: FATAL - malloc failed!\n");
		return(0);
	}
	
	strcpy(*date,cdate);
	
	/* here is the easier way */
	printf("Enter the item: ");
	*item = get_dynamic_str();
	if (!*item)
	{
		fprintf(stderr,"get_data: FATAL - get_dynamic_str failed!\n");
		return(0);
	}
	
	printf("Enter the price: ");
	scanf("%f",price);
	
	return(1);
}

int get_scores(int number,int **tests)
{
	int i=0;
	
	*tests = (int *) malloc(sizeof(int) * number);
	if (!*tests)
	{
		fprintf(stderr,"get_scores: FATAL - malloc failed!\n");
		return(0);
	}
	
	for (i = 0; i < number; i++)
	{
		printf("get score %d: ",i);
		scanf("%d",&((*tests)[i]) );
	}
	
	return(1);
}

void main()
{
	char *main_date=NULL, *main_item=NULL;
	float main_price = 0.0;
	int i=0,*test_array=NULL;
	
	/* we already know what the pointers hold. They all
	hold NULL because that is what we initialized
	them to. */
	
	/* PROBLEM: I want to "fill" the pointers with data but
	since I should program in a modular fashion I want a 
	separate function to do the work.  How do I get a 
	function to malloc space and fill pointers that
	I have declared in this function? 
	ANSWER: whenever you want to modify the value of a 
	variable inside a function you PASS (COPY) THE ADDRESS to the
	function.  Do the same thing for pointers. */
	
	/* let's look at the addresses of our variables. */
	printf("main_date is at address %p.\n",&main_date);
	printf("main_item is at address %p.\n",&main_item);
	printf("main_price is at address %p.\n",&main_price);
	printf("test_array is at address %p.\n",&test_array);
	
	if (!get_data(&main_date, &main_item, &main_price))
	{
		fprintf(stderr,"main: FATAL - get_data failed!\n");
		exit(0);
	}
	
	/* let's print out the data get_data provided. */
	printf("date: %s  item: %s  price: %3.2f\n",main_date, main_item,
											 main_price);
	
	/* let's get 10 scores */
	if (!get_scores(TESTNUM, &test_array))
	{
		fprintf(stderr,"main: FATAL - get_scores failed!\n");
		exit(0);
	}
	
	/* print out the scores */
	printf("test scores: ");
	for (i=0; i<TESTNUM; i++)
	{
		if (!i)
			printf("%d",test_array[i]);
		else if (i != (TESTNUM - 1))
			printf(",%d",test_array[i]);
		else
			printf(",%d\n",test_array[i]);
	}
}