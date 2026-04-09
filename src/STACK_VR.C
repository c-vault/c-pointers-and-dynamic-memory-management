/* stack_variable.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void func1(int num1, int *numptr, char char1, char *string)
{
	printf("the address of num1   on the stack is: %p\n",&num1);
	printf("the address of numptr on the stack is: %p\n",&numptr);
	printf("the address of char1  on the stack is: %p\n",&char1);
	printf("the address of string on the stack is: %p\n",&string);
	printf("num1   holds %d.\n",num1);
	printf("numptr holds %p.\n",numptr);
	printf("char1  holds %c.\n",char1);
	printf("string holds %p.\n",string);
}

void main()
{
	int main_int=50;
	char main_char='m';
	char main_name[] = "mike";
	
	printf("the address of main_int  is %p.\n",&main_int);
	printf("the address of main_char is %p.\n",&main_char);
	printf("the address of main_name is %p.\n",&main_name);
	printf("main_int  holds %d.\n",main_int);
	printf("main_char holds %c.\n",main_char);
	printf("main_name holds %s.\n",main_name);
	func1(main_int,&main_int,main_char,main_name);
}