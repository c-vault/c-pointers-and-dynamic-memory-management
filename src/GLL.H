#ifndef GLL_LIST
#define GLL_LIST

struct genericlist {
   int ns,ni,nf;
   char **Strings;
   int *Integers;
   float *Floats;
   struct genericlist *next;
};

typedef struct genericlist *glptr;

/* Here is the utility routines to create and manipulate generic linked lists.*/

glptr initgll(int numstr,int numint, int numflt,...);

int addgll_node(glptr start, ...);

void printgll();

glptr srchgll(glptr start, int numkeys, ...);

glptr delgll_node(glptr start, glptr searchptr);

int extractgll_node(glptr start, glptr searchptr, ...);

void delgll();

glptr sortgll(glptr start, int numkeys, ...);

#endif