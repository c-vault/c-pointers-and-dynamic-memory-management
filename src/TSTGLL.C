/* tstgll.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "gll.h"

void main()
{
	glptr studentgll=NULL,searchptr=NULL;

	char name[80], street[80], city[32], state[3];

	int zip, age;

	float grade;

	char pause[80];

	/* a list of student records */
	studentgll = initgll(4,2,1,"Jack Bright","222 Golden lane",
						"Tinseltown","LA",(double) 32341,
						(double) 21, (double) 3.9);
	if (!studentgll)
	{
		fprintf(stderr,"tstgll: FATAL - initgll failed!\n");
		exit(0);
	}

	if (!addgll_node(studentgll,"Cindy Society","323 Spruce st.","Holyoake","MA",
					(double) 22113, (double) 22, (double) 3.2))
	{
		fprintf(stderr,"tstgll: FATAL - addgll_node failed!\n");
		exit(0);
	}

	if (!addgll_node(studentgll,"John Green","323 Oak st.","Beantown","VT",
					(double) 10743, (double) 19, (double) 3.0))
	{
		fprintf(stderr,"tstgll: FATAL - addgll_node failed!\n");
		exit(0);
	}
	
	if (!addgll_node(studentgll,"Guy Studly","221 Boulder st.","Dolittle","DE",
					(double) 22543, (double) 20, (double) 2.5))
	{
		fprintf(stderr,"tstgll: FATAL - addgll_node failed!\n");
		exit(0);
	}
	
	printgll(studentgll,stdout);
	
	if ((searchptr = srchgll(studentgll,1,'s',(double) 1,"John Green")) == NULL)
	{
		printf("srchgll could not find John Green.\n");
	}
	else
		printf("srchgll found John Green\n");
		
	studentgll = delgll_node(studentgll,searchptr);
	
	if (!studentgll)
	{
		fprintf(stderr,"tstgll: FATAL - delgll_node failed!\n");
		exit(0);
	}

	printf("hit return");
	gets(pause);
	
	printgll(studentgll,stdout);
	
	printf("hit return");
	gets(pause);

	if ((searchptr = srchgll(studentgll,1,'s',(double) 1,"Cindy Society")) == NULL)
	{
		printf("srchgll could not find Cindy Society.\n");
	}
	else
		printf("srchgll found Cindy Society\n");

	if (!extractgll_node(studentgll,searchptr,name,street,city,state,&zip,&age,&grade))
	{
		fprintf(stderr,"tstgll: FATAL - extractgll_node failed!\n");
		exit(0);
	}
	
	printf("name is %s.\n",name);
	printf("street is %s.\n",street);
	printf("city is %s.\n",city);
	printf("state is %s.\n",state);
	printf("zip is %d.\n",zip);
	printf("age is %d.\n",age);
	printf("grade is %f.\n",grade);
	
	delgll(studentgll);	
}