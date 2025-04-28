/* changeprio.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void func1(char *arg1)
{
	printf("performing action 1 on %s.\n",arg1);
}

void func2(char *arg1)
{
	printf("performing action 2 on %s.\n",arg1);
}

void func3(char *arg1)
{
	printf("performing action 3 on %s.\n",arg1);
}

void func4(char *arg1)
{
	printf("performing action 4 on %s.\n",arg1);
}

void (*initial[4]) (char *arg1) = {func1, func2, func3, func4};
void (**action) (char *arg1);

void do_action(int type, char *instr)
{
	if ( (type >= 0) && (type <= 3) )
		(*action[type]) (instr);
}

void main()
{
	int i=0,j=0;
			
	action = (void (*) ()) malloc(sizeof(void (*) ()) * 4);
	for (i = 0; i < 4; i++)
			action[i] = initial[i];

	printf("Before priority adjustment.\n");	
	for (i = 0; i < 4; i++)
		do_action(i, "object");
		
	/* reverse priorities */
	for (i = 0, j = 3; i < 4; i++,j--)
		action[i] = initial[j];

	printf("After priority adjustment.\n");	
	for (i = 0; i < 4; i++)
		do_action(i, "object");
		
	
}