/* ppblock.h */

#define BLOCKSIZE 100

typedef struct ppblock ppblock;
typedef ppblock *ppblockp;
struct ppblock {
	char *name;
	char **ppbuf;
	int allocated;
	int used;
};
