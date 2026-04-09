/* generic_tree.h */

typedef struct generic_tree generic_tree;
typedef generic_tree *generic_treep;
struct generic_tree {
	short balance;
	short lheight;
	short rheight;
	short ns, nl, nd;
	char keytype;
	short keypos;
	char **Strings;
	long *Longs;
	double *Doubles;
	generic_treep left,right,parent;
};

