/* hyper_dict.h */
	
/*** Remove these horrible limitations 
#define SENTENCE_MAX 3
#define ENTRY_MAX 50
                      ***/

#define MINNUM 50
#define HYPERMIN 10

typedef struct dict_entry dict_entry;
typedef dict_entry *dict_entryp;
struct dict_entry {
	char *keyword;
	char *definition;
	int *hyper_links;
	int hyper_cnt;
	int hyper_max;
} **dictionary;

int entry_count=0;