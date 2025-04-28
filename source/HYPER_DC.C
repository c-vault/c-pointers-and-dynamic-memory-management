#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hyper_dc.h"

/* hyper_dict.c - a dynamic hypertext dictionary */

extern char *get_dynamic_str();
extern char *get_file();


/* get_definition - gets one definition and returns a malloced
entry. */
dict_entryp get_definition()
{
	dict_entryp out_entry;
	int i;

	/* malloc space for out_entry */
	out_entry = (dict_entryp) malloc(sizeof(dict_entry));
	if (!out_entry)
	{
		fprintf(stderr,"get_definition: FATAL - malloc failed!\n");
		return(NULL);
	}

	printf("\n Enter keyword: ");
	/* use get_dynamic_str() which is similar to gets but
	mallocs the string for us. */
	out_entry->keyword = get_dynamic_str();

	printf("\n Enter the definition: (ctrl-D when finished)\n");
	out_entry->definition = get_file();

	out_entry->hyper_cnt=0;
	out_entry->hyper_max = HYPERMIN;

	/* malloc the space for the hyper links */
	out_entry->hyper_links = (int *) malloc(sizeof(int) * HYPERMIN);
	if (!out_entry->hyper_links)
	{
		fprintf(stderr,"get_definition: FATAL - malloc failed!\n");
		return(NULL);
	}

	return(out_entry);
}

/* display_entry */
void display_entry(dict_entryp display_rec)
{
	int i;
	printf("Keyword: %s\n",display_rec->keyword);
	printf("%s\n",display_rec->definition);
	/* display the hyperlinks */
	if (display_rec->hyper_cnt)
	{
		printf(">>--HyperLinks-->\n");
		printf("0) exit to main menu.\n");
		for (i = 0; i < display_rec->hyper_cnt; i++)
		{
			printf("%d) %s\n",i+1,
			dictionary[display_rec->hyper_links[i]]->keyword);
		}
	}
}

/* list_entries */
void list_entries()
{
	int i;

	printf("Dictionary Entries\n");
	for (i=0; i < entry_count; i++)
		printf("%s\n",dictionary[i]->keyword);
	printf("****** End of Entries *******\n");
}

int find_entry(char *keyword)
{
	int i;

	for (i=0; i < entry_count; i++)
	{
		if (!(strcmp(keyword,dictionary[i]->keyword)))
			return(i);
	}
	return(-1);
}

void add_hyper_links()
{
	int i=0,target=0;
	int *temp=NULL;

	char *hyper_word=NULL;
	
	target = entry_count - 1;
	if (target)
	{
		hyper_word = dictionary[target]->keyword;
		for (i=0; i<target; i++)
		{
			if (strstr(dictionary[i]->definition,hyper_word))
			{
				/* update the hyper link of the entry */
				if (dictionary[i]->hyper_cnt == dictionary[i]->hyper_max)
				{
					temp = dictionary[i]->hyper_links;
					dictionary[i]->hyper_links = (int *) realloc(dictionary[i]->hyper_links,
					sizeof(int) * (dictionary[i]->hyper_max + HYPERMIN));
					if (!dictionary[i]->hyper_links)
					{
						fprintf(stderr,"add_hyper_links: realloc failed on %d bytes.\n",
						sizeof(int) * (dictionary[i]->hyper_max + HYPERMIN));
						dictionary[i]->hyper_links = temp;
					}
					else
						dictionary[i]->hyper_max += HYPERMIN;
				}
				
				if (dictionary[i]->hyper_cnt < dictionary[i]->hyper_max)
				{
					dictionary[i]->hyper_links[dictionary[i]->hyper_cnt] = target;
					dictionary[i]->hyper_cnt++;
				}
				else
					fprintf(stderr,"add_hyper_links: cannot allocate any more memory!\n");
				
				/* update the hyper link of the target */
				if (dictionary[target]->hyper_cnt == dictionary[target]->hyper_max)
				{
					temp = dictionary[target]->hyper_links;
					dictionary[target]->hyper_links = (int *) realloc(dictionary[target]->hyper_links,
					sizeof(int) * (dictionary[target]->hyper_max + HYPERMIN));
					if (!dictionary[target]->hyper_links)
					{
						fprintf(stderr,"add_hyper_links: realloc failed on %d bytes.\n",
						sizeof(int) * (dictionary[target]->hyper_max + HYPERMIN));
						dictionary[target]->hyper_links = temp;
					}
					else
						dictionary[target]->hyper_max += HYPERMIN;
				}
				
				if (dictionary[target]->hyper_cnt < dictionary[target]->hyper_max)
				{
					dictionary[target]->hyper_links[dictionary[target]->hyper_cnt] = i;
					dictionary[target]->hyper_cnt++;
				}
				else
					fprintf(stderr,"add_hyper_links: cannot allocate any more memory!\n");
	
			} /* if strstr */
		} /* for all entries */			
	} /* if target */
}

void main()
{
	int done=0;
	int choice=0,idx=0,entry_max=MINNUM;
	int hyper_choice=0,hyper_count=0;
	
	char display_kw[80];
	dict_entryp *temp;
	dict_entryp hyper_rec;
	
	/* malloc space for MINNUM of dictionary entries */
	dictionary = (dict_entryp *) malloc(sizeof(dict_entryp) * MINNUM);
	if (!dictionary)
	{
		fprintf(stderr,"hyper_dict: FATAL - malloc failed on %d bytes.\n",
		sizeof(dict_entryp) * MINNUM);
		exit(0);
	}
	
	while (!done)
	{
		printf("<<<< Hyper-Webster >>>>\n");
		printf("1) enter a definition.\n");
		printf("2) list all entries.\n");
		printf("3) display an entry.\n");
		printf("4) exit.\n");
		printf("choice: ");
		scanf("%d",&choice);
		fflush(stdin); /* this is so the \n gets flushed out
						   of the buffer */
		switch (choice) {
			case 1: 
				if (entry_count == entry_max)
				{
					/* realloc more space */
					temp = dictionary;
					dictionary = (dict_entryp *) realloc(dictionary,sizeof(dict_entryp)
					 * (entry_max + MINNUM));
					if (!dictionary)
					{
						fprintf(stderr,"hyper_dict: FATAL - realloc failed on %d bytes.\n",
						sizeof(dict_entryp) * (entry_max + MINNUM));
						dictionary = temp;
					}
					else
						entry_max += MINNUM;
				}	
				
				if (entry_count < entry_max)
				{
					dictionary[entry_count] = get_definition();
					if (!dictionary[entry_count])
					{
						fprintf(stderr,"hyper_dict: FATAL - get_definition failed!\n");
						exit(0);
					}
					entry_count++;
					/* add hyper links */
					add_hyper_links();
				}
				else
				{
					fprintf(stderr,"hyper_dict: Warning - unable to allocate any more memory!\n");
				}
				break;
			case 2:
				list_entries();
				break;
			case 3:
				printf("\n Enter keyword to display: ");
				gets(display_kw);
				if ((idx = find_entry(display_kw)) >= 0)
				{
					display_entry(dictionary[idx]);
					hyper_choice = 1;
					hyper_count = dictionary[idx]->hyper_cnt;
					while ( (hyper_choice != 0) && (hyper_count) )
					{
						/* jump to hyperlinks */
						printf("choice: ");
						scanf("%d",&hyper_choice);
						fflush(stdin); /* this is so the \n gets flushed out
							  of the buffer */
						if (hyper_choice)
						{
							hyper_rec = dictionary[dictionary[idx]->hyper_links[hyper_choice-1]];
							display_entry(hyper_rec);
							hyper_count = hyper_rec->hyper_cnt;
							idx = dictionary[idx]->hyper_links[hyper_choice-1];
						}
					}
				}
				else
					printf("\n %s not found.\n",display_kw);
				break;
			case 4:
				done = 1;
				break;
			default:
				printf("\nInvalid choice - try again.\n");
		} /* switch */
	} /* while not done */
}