/* globals.c */
#include <stdio.h>
#include <stdlib.h>

/* the next two variables will be stored in the global application space.  Any function within the file may read or write to these variables. */
int global_int; 
char global_char;

void main()
{
   int local_int;
   printf("this program does nothing except show the ");
   printf(" declaration of global and local variables\n");
}