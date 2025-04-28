/* funcptr.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long add_em(int a, int b)
{
	return((a+b));
}

long sub_em(int a, int b)
{
	return((a-b));
}

long mult_em(int a, int b)
{
	return((a*b));
}

void upcase(char **instr)
{
	int i,len;
	
	if (instr);
	{
		len = strlen(*instr);
		for (i=0; i < len; i++)
		{
			if (islower((*instr)[i]))
				(*instr)[i] = toupper((*instr)[i]);
		}
	}
}

char *reverse_em(char *instr)
{
	int i,j,len;
	
	char *outstr;
	
	if (instr)
	{
		len = strlen(instr);
		outstr = (char *) malloc(sizeof(char) * (len + 1));
		if (outstr)
		{
			for (i = 0, j = len - 1; i < len; i++, j--)
				outstr[i] = instr[j];
		}
		outstr[len] = '\0';
	}
	return(outstr);
}

void main()
{
	long (*math) (int a, int b);
	
	long (*math_array[3]) (int a, int b);
	
	enum ops {add,sub,mult};
	
	void (*fp) (char **str);
	
	char *(*fp2) (char *str);
	
	char *str1;
	
	int var1, var2, i, j;
	long answer;
	
	fp = upcase;
	fp2 = reverse_em;
	
	printf("address of upcase function is %p.\n",fp);
	printf("address of reverse_em function is %p.\n",fp2);
	
	str1 = (*fp2) ("junk");
	printf("after dereferencing fp2, str1 is %s.\n",str1);
	(*fp) (&str1);
	printf("after dereferencing fp, str1 is %s.\n",str1);
	
	math_array[add] = add_em;
	math_array[sub] = sub_em;
	math_array[mult] = mult_em;
	
	var1 = 1; var2 = 2;
	printf("var1 is %d, var2 is %d.\n",var1,var2);
	
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 3; j++)
		{
			answer = (*math_array[j]) (var1, var2);
			var1 = answer;
		}
		printf("answer is %ld.\n",answer);
	}
}
	