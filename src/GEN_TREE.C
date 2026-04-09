#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

#include "gen_tree.h"

int gcmp(char keytype, int keypos, char **Strings1, long *Longs1, double *Doubles1,
									char **Strings2, long *Longs2, double *Doubles2)
{	
	switch (keytype) {
		case 's':
			return (strcmp(Strings1[keypos],Strings2[keypos]));
			break;
		case 'l':
			if (Longs1[keypos] == Longs2[keypos])
				return(0);
			else
				return( Longs1[keypos] > Longs2[keypos] ? 1 : -1);
			break;
		case 'd':
			if ( Doubles1[keypos] == Doubles2[keypos] )
				return(0);
			else
				return( Doubles1[keypos] > Doubles2[keypos] ? 1 : -1);
			break;
	} /* end of switch */
}

generic_treep init_gtree(char keytype, short keypos, 
						short nums, short numl, short numd, ...)
{
   generic_treep head;
   int i,j,k;
   va_list args;
   char *shold;
   long lhold;
   double dhold;
 
   if ( (nums<0) || (numl<0) || (numd<0) )
   {
      fprintf(stderr,"initgll: input paramaters invalid.\n");
      return(NULL);
   }
   else
      head = (generic_treep) malloc(sizeof(struct generic_tree));
      
    if (!head)
    {
       fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
       return(NULL);
    }
   
   head->balance=0;
   head->lheight=0;
   head->rheight=0;
   head->keytype = keytype;
   head->keypos = keypos;
  
   va_start(args,numd);
   
   /* store the counts in the structure. */
   head->ns = nums; head->nl = numl; head->nd = numd;
   
   if (nums>0)
   {
      head->Strings = (char **) malloc(sizeof(char *) * nums);
      if (!head->Strings)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (i = 0; i < nums; i++)
      {
 		 shold = va_arg (args,char *);
		 head->Strings[i] = (char *)malloc(sizeof(char) * (strlen(shold) + 1));
		 strcpy(head->Strings[i], shold);
      }
   }
   
   if (numl>0)
   {
      head->Longs = (long *) malloc(sizeof(long) * numl);
      if (!head->Longs)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (j = 0; j < numl; j++)
      {
	   lhold = (long) va_arg(args,double);
	   head->Longs[j] = lhold;
      }
   }
      
   if (numd>0)
   {
      head->Doubles = (double *) malloc(sizeof(double) * numd);
      if (!head->Doubles)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(NULL);
      }
      
      for (k = 0; k < numd; k++)
      {
  	   dhold = (double) va_arg(args,double);
	   head->Doubles[k] = dhold;
      }
   }
   
   va_end(args);
   
   /* now assign the children pointers to null */

	head->left = NULL;
	head->right = NULL;
	head->parent = NULL;
	
   /* now return the head pointer */

   return(head);
}

int add_node(generic_treep root, ...)
{
	generic_treep new=NULL,traverse=NULL,predecessor=NULL;
	int nums, numl, numd,i,j,k,stat;
	char keytype;
	short keypos;
	char *shold;
	long lhold;
	double dhold;
	va_list args;
	short tot_height=0;
		
	if (!root)
	{
		fprintf(stderr,"add_node: FATAL - root may not be NULL!\n");
		return(0);
	}
	
	/* malloc space for the new node */
	new = (generic_treep) malloc(sizeof(generic_tree));
	if (!new)
	{
		fprintf(stderr,"add_node: FATAL - malloc failed!");
		return(0);
	}
	
	/* read the counts from the root */
	keytype = new->keytype = root->keytype;
	keypos = new->keypos = root->keypos;
	nums = new->ns = root->ns;
	numl = new->nl = root->nl;
	numd = new->nd = root->nd;
	
	/* read in all of the variable arguments */
	va_start(args,root);
	
  if (nums>0)
   {
      new->Strings = (char **) malloc(sizeof(char *) * nums);
      if (!new->Strings)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(0);
      }
      
      for (i = 0; i < nums; i++)
      {
 		 shold = va_arg (args,char *);
		 new->Strings[i] = (char *)malloc(sizeof(char) * (strlen(shold) + 1));
		 strcpy(new->Strings[i], shold);
      }
   }
   
   if (numl>0)
   {
      new->Longs = (long *) malloc(sizeof(long) * numl);
      if (!new->Longs)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(0);
      }
      
      for (j = 0; j < numl; j++)
      {
	   lhold = (long) va_arg(args,double);
	   new->Longs[j] = lhold;
      }
   }
      
   if (numd>0)
   {
      new->Doubles = (double *) malloc(sizeof(double) * numd);
      if (!new->Doubles)
      {
	   fprintf(stderr,"init_gtree: error mallocing structure memory.\n");
	   return(0);
      }
      
      for (k = 0; k < numd; k++)
      {
  	   dhold = (double) va_arg(args,double);
	   new->Doubles[k] = dhold;
      }
   }
   
   va_end(args);
   
   /* now assign the children pointers to null */
	new->left = NULL;
	new->right = NULL;

	/* now traverse the tree */
	traverse = root;
	while (traverse)
	{
		if ( (stat = gcmp(keytype,keypos,new->Strings,
									new->Longs,
									new->Doubles,
									traverse->Strings,
									traverse->Longs,
									traverse->Doubles)) > 0)
		{
			if (traverse->right)
			{
				tot_height++;
				traverse = traverse->right;
			}
			else
			{
				/* add node here */
				tot_height++;
				traverse->rheight++;
				traverse->balance = traverse->rheight - traverse->lheight;
				traverse->right = new;
				new->parent = traverse;
				new->balance = 0;
				new->lheight=0;
				new->rheight=0;
				traverse=NULL; /* terminate loop */
			}
		}
		else
		{
			if (traverse->left)
			{
				tot_height++;
				traverse = traverse->left;
			}
			else
			{
				/* add node here */
				tot_height++;
				traverse->lheight++;
				traverse->balance = traverse->rheight - traverse->lheight;
				traverse->left = new;
				new->parent = traverse;
				new->balance = 0;
				new->rheight=0;
				new->lheight=0;
				traverse=NULL; /* terminate loop */
			}
		}
	} /* end of while */			

	/* traverse back up the tree and insure the balance counts are correct. */
	predecessor=new;
	for (i=1; i <= tot_height; i++)
	{
		traverse = predecessor;
		predecessor = predecessor->parent;
		if ( (stat = gcmp(keytype,keypos,traverse->Strings,
									traverse->Longs,
									traverse->Doubles,
									predecessor->Strings,
									predecessor->Longs,
									predecessor->Doubles)) > 0)
		{
			/* a right node */
			if (predecessor->rheight < i)
			{
				predecessor->rheight = i;
				predecessor->balance = predecessor->rheight - predecessor->lheight;
			}
		}
		else
		{
			/* a left node */
			if (predecessor->lheight < i)
			{
				predecessor->lheight = i;
				predecessor->balance = predecessor->rheight - predecessor->lheight;
			}			
		}
	} /* end of for */		

	return(1);
}

void print_key(char keytype, int keypos,short balance,short lheight, short rheight, 
char **Strings, long *Longs, double *Doubles)
{
	switch (keytype) {
		case 's':
			printf("K:%s B:%d L:%d R:%d\n",Strings[keypos],balance,lheight,rheight);
			break;
		case 'l':
			printf("K:%ld B:%d L:%d R:%d\n",Longs[keypos],balance,lheight,rheight);
			break;
		case 'd':
			printf("K:%f B:%d L:%d R:%d\n",Doubles[keypos],balance,lheight,rheight);
		break;
	}
}

void print_tree_keys(generic_treep r, int l)
{
	int i;
	
	if (!r) return;
	
	print_tree_keys(r->left, l+1);
	for (i = 0; i < l;++i) printf("         ");
	print_key(r->keytype,r->keypos,r->balance,r->lheight, r->rheight,
	r->Strings,r->Longs,r->Doubles);
	print_tree_keys(r->right,l+1);
}

void main()
{
	char s[80];
	long int num=0;
	
	generic_treep root = NULL;
	
	do {
		printf("enter a number: ");
		gets(s);
		num = atol(s);
		
		if (s[0])
		{
			if (!root) root = init_gtree('l',0,0,1,0,(double)num);
			else add_node(root,(double)num);
		}
	} while (s[0]);
	
	print_tree_keys(root,0);
}