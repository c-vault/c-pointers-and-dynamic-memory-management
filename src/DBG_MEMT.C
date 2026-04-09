#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "memory.h"

void main()
{
	char *str1, *str2, *str3, *str4, *str5;
	char *never_malloced;
	unsigned blocks, largest;
	int i,len;
	char bad_string[] = "this is a string too long to be stuffed into st1.";

	printf("case 1: malloc memory of varying sizes.\n");
	str1 = strdup("This is sentence one.");
	str2 = (char *) malloc(400);
	str3 = strdup("This is sentence Three and rather lengthy and long and very long.");
	str4 = (char *) malloc(600);
	str5 = strdup("This is sentence Five.");

	if (!str1 || !str2 || !str3 || !str4 || !str5)
	{
		printf("malloc failed!\n");
		exit(0);
	}
	
	memory_map();
	print_free_list();	
	print_stats();	

	printf("case 2: free some memory.\n");
	free(str2);
	free(str4);

	memory_map();
	print_free_list();	
	print_stats();	

	printf("case 3: test free on errors.\n");
	free(NULL);
	free(str4);
	never_malloced = str1 + 5;
	free(never_malloced);
	
	printf("case 4: free memory and coalesce small blocks into big blocks.\n");
	free(str3);

	memory_map();
	print_free_list();	
	print_stats();	
	
	printf("case 5: check the heap for corruption. Should be good.\n");
	if (!check_heap())
		printf("HEAP IS CORRUPTED!\n");
	else
		printf("HEAP IS OK!\n");
		
	printf("case 6: corrupt the heap!\n");
	
	/* we will corrupt the heap by overwriting str3. */
	str2 = strdup("this is string2.");
	str3 = strdup("this is string3.");
	str4 = strdup("this is string4.");
	strcpy(str2,bad_string);
				
	if (!check_heap())
		printf("HEAP IS CORRUPTED!\n");
	else
		printf("HEAP IS OK!\n");

	memory_map();
	print_free_list();	
	print_stats();	

}