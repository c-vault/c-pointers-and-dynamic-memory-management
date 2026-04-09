/* debug_memory.h */

#ifndef _DEBUG_MEMORY
#define _DEBUG_MEMORY
/* Alignment defines */
struct fooalign {char x; double d;};
#define DEFAULT_ALIGNMENT ((char *) &((struct fooalign *) 0)->d - (char *) 0)
/* NB: DEFAULT_ALIGNMENT macro is from GNU-C source by the Free Software Foundation */

#define MULTIPLE_OF(x) ( ((x)%DEFAULT_ALIGNMENT) ? \
					     ((DEFAULT_ALIGNMENT - ((x)%DEFAULT_ALIGNMENT)) + (x)) : (x))

/* header block */
typedef struct header header;
typedef header *headerp;
struct header {
	unsigned header_magic;
	headerp next;
	unsigned size;
};

/* Start & End Bytes */
#define START_BYTES (MULTIPLE_OF(sizeof(header)))
#define END_BYTES (MULTIPLE_OF(sizeof(unsigned)))
/* s is the number of bytes the user requested */
#define TOT_BYTES(s) (MULTIPLE_OF(s) + START_BYTES + END_BYTES)
#define END_OFFSET(s) (MULTIPLE_OF(s) + START_BYTES)
/* x is a total number of bytes available, like from get_mem */
#define USABLE_BYTES(x) (x - (START_BYTES + END_BYTES))

/* magic numbers */
#define MAGIC_MALLOC 0x1212
#define MAGIC_FREE 0x2323
#define MALLOCFLAG 1
#define FREEFLAG 0

/* macros */
#define ret_addr(ptr) ( (void *) ((char *)ptr + START_BYTES) )
#define header_magic(ptr) (((headerp)((char *)ptr - START_BYTES))->header_magic)
#define header_size(ptr) (((headerp)((char *)ptr - START_BYTES))->size)
#define end_magic(ptr) (* (unsigned *) ((char *)ptr + MULTIPLE_OF(header_size(ptr))))

/* default sizes */
#define DEFAULT_BLOCK_SIZE 1024
#define DEBUG_BOUND 65534

/* free list */
static header freelist; /* freelist */
static headerp lfree = NULL; /* last freed block */

/* malloc statistics */
struct heap_info {
	unsigned long current_heap_size;
	unsigned free_blocks;
	unsigned used_blocks;
	unsigned largest_free_block;
	unsigned long total_free_mem;
} stats = {0,0,0,0,0};

/* debug routines */
void set_mem(char *ptr, unsigned size,short flag);
headerp get_mem(unsigned size);
int check_magic(headerp ptr, short flag);
int check_free_list(unsigned *largest);
void print_free_list(void);
void memory_map(void);
int check_heap(void);
void *dbg_malloc(unsigned num_bytes);
void dbg_free(void *ptr);
char *dbg_strdup(char *instr);
void *dbg_realloc(void *ptr, long size);
#endif