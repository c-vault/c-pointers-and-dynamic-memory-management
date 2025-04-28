#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_stack.h"

extern ppstackp init_ppstack();
extern int pushstr();
extern char *popstr();
extern void print_ppstack();
extern void free_ppstack();
extern int mpush(ppstackp thestack,...);
extern char **mpop();

main()
{
   ppstackp test_stack;
   char *data;
   char **mdata;
   
   char buf[80];
   
   test_stack = init_ppstack();
   if (!test_stack)
   {
      fprintf(stderr,"init_ppstack failed!\n");
      exit(0);
   }

   printf("test_stack initialized!\n");

   if (!pushstr(test_stack,"mike"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }

   if (!pushstr(test_stack,"frank"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }

   if (!pushstr(test_stack,"Kris"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }
   if (!pushstr(test_stack,"Joe"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }
   
   if (!pushstr(test_stack,"John"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }

   if (!pushstr(test_stack,"chris"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }
   
   if (!pushstr(test_stack,"cj"))
   {
      fprintf(stderr,"pushstr failed!\n");
      exit(0);
   }      

   print_ppstack(test_stack);

	printf("type anything and press return\n");
	scanf("%s",buf);

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   print_ppstack(test_stack);
   
   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   print_ppstack(test_stack);

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   print_ppstack(test_stack);

   data = popstr(test_stack);
   if (data)
      printf("popped data is %s\n",data);
   else
      printf("popstr failed!\n");

   print_ppstack(test_stack);

	printf("type anything and press return\n");
	scanf("%s",buf);

   printf("multiple push ... \n");
   mpush(test_stack,"mike","frank","joe","john","cj",NULL);
   print_ppstack(test_stack);

   printf("multiple pop ... \n");
   mdata = mpop(test_stack,2);
   print_ppstack(test_stack);

   printf("multiple pop ... \n");
   mdata = mpop(test_stack,2);
   print_ppstack(test_stack);
   
   printf("multiple push ... \n");
   mpush(test_stack,"lori","kristine",NULL);
   print_ppstack(test_stack);

	printf("type anything and press return\n");
	scanf("%s",buf);
   
   printf("multiple pop ... \n");
   mdata = mpop(test_stack,2);
   print_ppstack(test_stack);

   printf("multiple pop ... \n");
   mdata = mpop(test_stack,2);
   print_ppstack(test_stack);
}
