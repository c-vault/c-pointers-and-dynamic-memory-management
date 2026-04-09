/* generic_compare.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int numcmp(char *a, char *b)
{
	double v1,v2;
	
	v1 = atof(a);
	v2 = atof(b);
	
	if (v1==v2)
		return (0);
	else
		return( (v1<v2) ? -1:1);
}

void swap(char **a, char **b)
{
	char *temp;
	
	temp = *a; *a = *b; *b = temp;
}

/* here is a generic insertion sort. */
void isort(int (*cmp)(char *, char *), char **data, int length)
{
	int i,j;
	
	for (i = 2; i < length; i++)
	{
		j = i;
		while ( (j > 0) && ( ((*cmp)(data[j],data[j-1])) == -1 ))
		{
			swap(&(data[j]), &(data[j-1]));
			j--;
		}
	}
}

void main()
{
	char *ages[10] = { "23", "56", "45", "55", "87", "12", "44", "99", "10", "17"};
	char *names[10] = { "john","joe","mike", "bill","bob","mack","mary","alice",
	"margaret","lynne"};
	int i;
	
	isort((int (*) (char *, char *)) strcmp, names, 10);
	for (i = 0; i < 10; i++) printf("%s ",names[i]);
	printf("\n");
	
	isort((int (*) (char *, char *)) numcmp, ages, 10);
	for (i = 0; i < 10; i++) printf("%s ",ages[i]);
	printf("\n");

}
	
	