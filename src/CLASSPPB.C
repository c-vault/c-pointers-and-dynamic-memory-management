/* class_ppbuf.c */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern char *get_dynamic_str();

void main()
{
	int **class_grades=NULL;
	char **student_names=NULL;
	int num_students=0;
	int num_tests=0;
	int i=0,j=0;
	
	printf("Enter the number of students in your class: ");
	scanf("%d",&num_students);
	printf("Enter the number of tests you gave this semester: ");
	scanf("%d",&num_tests);
	
	if (!num_students || !num_tests)
	{
		fprintf(stderr, "You entered 0 for one of the fields. Goodbye.\n");
		exit(0);
	}
	
	/* malloc the space for ppbufs */
	student_names = (char **) malloc(sizeof(char *) * num_students + 1);
	if (!student_names)
	{
		fprintf(stderr, "class_ppbuf: FATAL - malloc failed!\n");
		exit(0);
	}
	
	class_grades = (int **) malloc(sizeof(int *) * num_students + 1);
	if (!class_grades)
	{
		 fprintf(stderr,"class_ppbuf: FATAL - malloc failed!\n");
		 exit(0);
	}
	
	for (i=0; i<num_students;i++)
	{
		fflush(stdin);
		printf("Enter name of student %d: ",i);
		student_names[i] = get_dynamic_str();
		
		/* malloc space for the 1d int array */
		class_grades[i] = (int *) malloc(sizeof(int) * num_tests);
		if (!class_grades[j])
		{
			fprintf(stderr,"class_ppbuf: FATAL - malloc failed!\n");
			exit(0);
		}

		for (j=0; j<num_tests; j++)
		{
			
			printf("Enter the students grade for test %d: ",j);
			scanf("%d",&(class_grades[i][j]));
		}
	}
	
	/* print out what we stored. See Exercises 1 and 2 to expand
	upon this program.  We will print out the information
	differently than we received it. */
	for (i=0; i < num_tests; i++)
	{
		printf("On test #%d...\n",i);
		for (j=0; j < num_students; j++)
			printf("\t%s scored %d.\n",student_names[j],class_grades[j][i]);
	}
}
	