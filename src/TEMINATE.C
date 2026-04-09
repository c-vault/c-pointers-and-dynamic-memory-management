/* terminator.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gll.h"
#include "memory.h"
#include "simple_c.h"

extern char *get_dynamic_str();
extern char *get_c_token();
extern char *type_str();
extern int check_heap();


void main()
{
	char *source=NULL,*token=NULL,*desc=NULL,*this_array=NULL;
	char *target_array=NULL;

	FILE *fp=NULL;

	int last_token, token_type,dimension[10],num_dim=0;
	int i=0,target_dimension[10],target_ndim,qlen;
	int statement_cnt=0;

	glptr array_list=NULL,searchptr=NULL;
	short copyflag = 0, funcflag = 0;


	/* get the source file */
	printf("Source file to check for array overrun and termination: ");
	source = get_dynamic_str();
	if (!source)
		exit(0);

	if ( (fp = fopen(source,"r"))== NULL)
	{
		fprintf(stderr,"unable to open %s.\n",source);
		exit(0);
	}
	
	for (i = 0; i < 10; i++)
		dimension[i] = 0;
	
	while ( (token = get_c_token(fp,&token_type,last_token)) != NULL)
	{
		last_token = token_type;
		desc = type_str(token_type);
		switch (token_type)
		{
			case ARRAY:
				if (this_array)
				{
					/* store this array in gll before collecting next one */
					if (!array_list)
					{
						if (num_dim < 5)
							array_list = initgll(1,6,0,this_array,num_dim,dimension[0],
										dimension[1],dimension[2],dimension[3],
										dimension[4]);
						else
						{
							fprintf(stderr,"array_dimensioned over 5.");
							exit(0);
						}
					}
					else
					{
						if (num_dim < 5)
						{
							if (!addgll_node(array_list,this_array,num_dim,
								dimension[0],dimension[1],dimension[2],
								dimension[3],dimension[4]))
							{
								fprintf(stderr,"addgllnode failed!\n");
								exit(0);
							}
						}
						else
						{
							fprintf(stderr,"array_dimensioned over 5.");
							exit(0);
						}
					}
					/* get ready for next array entry */
					free(this_array); this_array = NULL;
					for (i =0; i < 10; i++)
						dimension[i] = 0;
				}
						
				this_array = strdup(token);
			break;
			case ARRAY_DIM_VARIABLE:
				dimension[num_dim] = atoi(token);
				num_dim++;
			break;
			case FUNCTION:
				funcflag = 1;
				if ( (!strcmp(token,"strcpy")) ||
					 (!strcmp(token,"strncpy")) )
					 copyflag = 1;
			break;
			case QUOTE:
				if (copyflag && target_array)
				{
					qlen = strlen(token);		
					if (target_ndim == 1)
					{
						if (qlen > target_dimension[0])
						{
							printf("**** OVERRUN ATTEMPT! ****\n");
							printf("In the copy function in statement number %d.\n",statement_cnt);
							printf("\"%s\" attempted to overrun %s array!\n",
									token,target_array);
							printf("**************************\n");
						}
					}
					else
					{
						/* handle multi-dimension case */
					}
				}
						
			break;
			case VARIABLE:
				if (copyflag || funcflag)
				{
					if (array_list)
					{
						searchptr = srchgll(array_list,1,'s',(double)0,token);
						/* if found */
					}
					else
					{
						/* may only have one array */
						if (this_array)
						{
							if (!strcmp(token,this_array))
							{
								/* MATCH!, this is a target of a function
								or a copy! */
								target_array = strdup(this_array);
								target_ndim = num_dim;
								for (i=0; i < 10; i++)
									target_dimension[i] = dimension[i];
							}
						}
					}
				}			
			break;
			case STATEMENT_END:
				statement_cnt++;
				copyflag = 0; funcflag = 0;
			break;
			default:
			break;
		}

		if (token) free(token); token = NULL;
		if (desc) free(desc); desc = NULL;
		if (!check_heap())
		{
			fprintf(stderr,"HEAP CORRUPTED! ABORT!\n");
			exit(0);
		}
		
	}
	if (source) free(source); source=NULL;
	memory_map();
	fclose(fp);
}