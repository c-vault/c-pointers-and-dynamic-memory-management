/* struct_tst.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct employee {
	char name[80];
	int age;
	char address[256];
};

void func1(struct employee *employee_ptr)
{
	strcpy(employee_ptr->name,"harry houdini");
}

void func2(struct employee employee_copy)
{
	strcpy(employee_copy.name,"Theodore Roosevelt");
	printf("my emloyee has a new name %s.\n",employee_copy.name);
}

void main()
{
	struct employee joe = {"joe blow",32,"222 main street, nowhere, US"};
	
	struct employee *eptr;
	
	printf("the starting address of joe is %p.\n",&joe);
	eptr = &joe;
	printf("eptr holds %p.\n",eptr);
	printf("the full name of joe is %s.\n",joe.name);
	printf("the full name of joe is %s.\n",eptr->name);
	printf("the full name of joe is %s.\n",(*eptr).name);
	
	/* we can copy the address of the joe structure to the stack,
	   for func1 */
	func1(&joe);
	
	/* func1 changed the name of our employee */
	printf("the full name of joe is now %s.\n",joe.name);
	printf("the full name of joe is now %s.\n",eptr->name);
	
	/* we can copy the entire structure to the stack for func2 */
	func2(joe);

	/* func2 DID NOT CHANGE the name of our employee */
	printf("the full name of joe is still %s.\n",joe.name);
	printf("the full name of joe is still %s.\n",eptr->name);	
}