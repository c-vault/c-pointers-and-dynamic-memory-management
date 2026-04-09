/* tokenize_it.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "memory.h"

extern char *get_dynamic_str();
extern char *get_c_token();
extern char *type_str();
extern int check_heap();
extern void memory_map();
extern void print_free_list();

void main()
{
	char *source=NULL,*token=NULL,*desc=NULL;
	FILE *fp=NULL;
	int last_token, token_type;
	
	/* get the source file */
	printf("Source file to tokenize: ");
	source = get_dynamic_str();
	if (!source)
		exit(0);
	
	if ( (fp = fopen(source,"r"))== NULL)
	{
		fprintf(stderr,"unable to open %s.\n",source);
		exit(0);
	}
		
	while ( (token = get_c_token(fp,&token_type,last_token)) != NULL)
	{
		last_token = token_type;
		desc = type_str(token_type);
		if (desc)
			printf("TOKEN %d (%s) is %s.\n",token_type,token,desc);
		if (token) free(token); token = NULL;
		if (desc) free(desc); desc = NULL;
		if (!check_heap())
		{
			fprintf(stderr,"HEAP CORRUPTED! ABORT!\n");
			exit(0);
		}
		
	}
	if (source) free(source); source=NULL;
	print_free_list();
	memory_map();
	fclose(fp);
}