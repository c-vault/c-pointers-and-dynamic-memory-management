#define STACK_GROW 50

typedef struct ppstack ppstack;
typedef ppstack *ppstackp;
struct ppstack {
   char **queue;
   int top;
   int stack_qty;
   int stack_max;
};
