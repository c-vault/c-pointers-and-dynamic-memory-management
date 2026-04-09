/* BETTER_multiple_exit.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "memory.h"
extern char *strdup(char *instr);
extern void memory_map();

void multi_free(char **first, ...)
{
	char **next;
	va_list args;
	
	if (first)
	{
		if (*first) free(*first);
		if (*first) *first = NULL;
		va_start (args, first);
		while (next = va_arg(args, char **))
		{
			if (*next) free(*next);
			if (*next) *next = NULL;
		}
	}
}

char *int2str(int number)
{
	int stat=0;
	char xfer[80];
	char *outbuf;
	
	if ( (stat = sprintf(xfer,"%d",number) == EOF) )
	{
		fprintf(stderr,"int2str: FATAL - sprintf failed on %d.\n",number);
		return(NULL);
	}
	
	outbuf = strdup(xfer);
	return(outbuf);
}

int work_func(int num1, int num2, int num3)
{
	char *str1=NULL, *str2=NULL, *str3=NULL;
	
	str1 = int2str(num1);
	str2 = int2str(num2);
	str3 = int2str(num3);
	
	if (num1 > num2)
	{
		fprintf(stderr,"workfunc Error: num1 <%d> greater than num2 <%d>.\n",num1,num2);
		multi_free(&str1,&str2,&str3,NULL);
		return(0);
	}
	else if (num2 > num3)
	{
		fprintf(stderr,"workfunc Error: num1 <%d> greater than num2 <%d>.\n",num2,num3);
		multi_free(&str1,&str2,&str3,NULL);
		return(0);
	}
	else if (num3 > 1000)
	{
		fprintf(stderr,"workfunc Error: num3 <%d> greater than 1000.\n",num3);
		multi_free(&str1,&str2,&str3,NULL);
		return(0);
	}

	printf("Number:%s%s%s\n",str1,str2,str3);
	multi_free(&str1,&str2,&str3,NULL);
	return(1);
}

void main()
{
	int i=0;
	int test1[5] = {100, 10, 30, 80, 500 };
	int test2[5] = { 99, 50,300, 90, 200 };
	int test3[5] = {101,100,200,300,1100 };
	
	for (i=0; i < 5; i++)
	{
		if (!work_func(test1[i],test2[i],test3[i]))
			printf("try %d, work_func failed!\n",i+1);
		else
			printf("try %d, work_func succeeded.\n",i+1);
	}
	memory_map();
}