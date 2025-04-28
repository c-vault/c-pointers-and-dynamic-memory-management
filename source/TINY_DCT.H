/* tiny_dict.h */
	
#define SENTENCE_MAX 3
#define ENTRY_MAX 50

typedef struct dict_entry dict_entry;
typedef dict_entry *dict_entryp;
struct dict_entry {
	char keyword[80];
	char sentence[SENTENCE_MAX][80];
} dictionary[ENTRY_MAX];

int entry_count=0;