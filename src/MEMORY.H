/* memory.h */

/* debug routines */
int check_free_list(unsigned *largest);
void print_free_list(void);
void memory_map(void);
int check_heap(void);
void *dbg_malloc(unsigned num_bytes);
void dbg_free(void *ptr);
char *dbg_strdup(char *instr);

#define malloc(x) dbg_malloc(x)
#define free(x) dbg_free(x)
#define strdup(x) dbg_strdup(x)
