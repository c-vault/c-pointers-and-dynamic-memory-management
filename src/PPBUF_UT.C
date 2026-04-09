/* ppbuf_utils */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "str_stck.h"

extern ppstackp init_ppstack();
extern int mpush(ppstackp thestack, ...);
extern char **mpop();

extern char *strdup();

/* #define DEBUGPP */

char **file2ppbuf(char *filename)
{
	FILE *fp;
	
	char str[512];
	char **buf=NULL;
	
	int linechunk=100;
	int cnt=0, len=0;
	
	if (!filename)
	{
		fprintf(stderr,"file2ppbuf:FATAL - filename is BULL!\n");
		return(NULL);
	}
	
	if ((fp = fopen(filename,"r"))==NULL)
	{
		fprintf(stderr,"file2ppbuf: FATAL - cannot open %s!\n",filename);
		return(NULL);
	}
	
	buf = (char **) malloc(sizeof(char *) * linechunk);
	if (!buf)
	{
		fprintf(stderr,"file2ppbuf: FATAL - malloc failed!\n");
		return(NULL);
	}
	
	cnt = 0;
	while (!feof(fp))
	{
		if (fgets (str,500,fp))
		{
			len = strlen(str);
			buf[cnt] = (char *) malloc(sizeof(char) * len + 1);
			strcpy(buf[cnt],str);
			cnt++;
			if (cnt >= linechunk)
			{
				/* realloc in quantities of 100 */
				buf = (char **) realloc(buf,sizeof(char *) * (cnt + 100));
				if (!buf)
				{
					fprintf(stderr,"file2ppbuf: FATAL - realloc failed!\n");
					return(NULL);
				}
				
				linechunk += 100;
			}
		}
	}
	
	buf[cnt] = NULL;
	
	fclose(fp);
	return(buf);
}

void free_ppbuf(char **inbuf, int count)
{
	int i;
	
	if (inbuf)
	{
		if (count>0)
		{
			for (i = 0; i < count; i++)
				if (inbuf[i]) free(inbuf[i]);
			free(inbuf);
		}
		else
		{
			for (i = 0; inbuf[i]; i++)
				count++;
			for (i = 0; i < count; i++)
				if (inbuf[i]) free(inbuf[i]);
			free(inbuf);
		}
	}
}

char **expandppbuf(char **inbuf, int num_lines)
{
	char **outbuf=NULL;
	static int elen=0;
	
	if (!inbuf)
	{
		outbuf = (char **) malloc(sizeof(char *) * (num_lines+1));
		if (!outbuf)
		{
			fprintf(stderr,"expabdppbuf: FATAL - malloc failed!\n");
			return(NULL);
		}
		elen = num_lines;
	}
	else
	{
		for (elen=0; inbuf[elen]; elen++);
		outbuf = (char **) realloc(inbuf, sizeof(char *) * (elen + (num_lines+1)));
		if (!outbuf)
		{
			fprintf(stderr,"expabdppbuf: FATAL - malloc failed!\n");
			return(NULL);
		}
	}
	
	return(outbuf);
}

/* if filename is NULL - dumps to stdout.
   if num_lines is 0 - expects ppbuf to be NULL-terminated */
int dumpppbuf(char **ppbuf, char *filename, int num_lines, int addnewline)
{
	int i=0;
	FILE *fp=NULL;
	
	if (!ppbuf)
	{
		fprintf(stderr,"dumpppbuf: FATAL - ppbuf cannot be NULL!\n");
		return(0);
	}
	
	if (filename)
	{
		if ( (fp = fopen(filename,"w")) == NULL)
		{
			fprintf(stderr,"dumpppbuf: FATAL - Unable to open %s!\n",filename);
			return(0);
		}
	}
	else
		fp = stdout;
		
	if (num_lines > 0)
	{
		for (i = 0; i < num_lines; i++)
		{
			if (ppbuf[i])
			{
				if (addnewline)
					fprintf(fp,"%s\n",ppbuf[i]);
				else
					fprintf(fp,"%s",ppbuf[i]);
			}
		}
	}
	else
	{
		for (i=0; ppbuf[i]; i++)
		{
			if (addnewline)
				fprintf(fp,"%s\n",ppbuf[i]);
			else
				fprintf(fp,"%s",ppbuf[i]);
		}
	}
	
	fclose(fp);
	return(1);
}

char **dupppbuf(char **ppbuf, int count)
{
	char **outbuf=NULL;
	int i=0, outcount=0;
	
	if (ppbuf)
	{
		if (count > 0)
		{
			outcount = count;
			/* This code originally had a bug in it.  The
			original version was
			outbuf = (char **) malloc(sizeof(char *) * count + 1);
			This is a bug of not requesting the right amount of memory!
			phil from Symantec set me straight. Thanks phil! */
			outbuf = (char **) malloc(sizeof(char *) * (count + 1));
			if (!outbuf)
			{
				fprintf(stderr,"dupppbuf: FATAL - malloc failed!\n");
				return(NULL);
			}
			
			for (i=0; i < count; i++)
				outbuf[i] = strdup(ppbuf[i]);
				
		}
		else
		{
			for (i = 0; ppbuf[i]; i++)
				outcount++;
				
			outbuf = (char **) malloc(sizeof(char *) * outcount + 1);
			if (!outbuf)
			{
				fprintf(stderr,"dupppbuf: FATAL - malloc failed!\n");
				return(NULL);
			}

			for (i = 0; i < (outcount+1); i++)
				outbuf[i] = NULL;
				
			for (i = 0; ppbuf[i]; i++)
				outbuf[i] = strdup(ppbuf[i]);
		}
		
		outbuf[outcount] = NULL;
	}
	return(outbuf);
}

int count_lines(char **inbuf)
{
	int i=0;
	
	if (inbuf)
	{
		for (i=0; inbuf[i]; i++)
			;
	}
	return(i);
}

void strip_trailing_whitespace(char **inbuf)
{
	int i=0,j=0,end=0;
	int valid_char=0,len=0;
	
	if (!inbuf)
	{
		fprintf(stderr,"strip_trailing_whitespace: FATAL - inbuf is NULL!\n");
		return;
	}
	
	for (i=0;inbuf[i]; i++)
		;
	
	end = i-1;
	
	/* strip until you hit a valid char */
	for (i=end; i >=0; i--)
	{
		len = strlen(inbuf[i]);
		for(j=0; j < len; j++)
		{
			if ( (isalnum(inbuf[i][j])) || (ispunct(inbuf[i][j])) )
				valid_char = 1;
			
			if (valid_char) break;
		}
		
		if (!valid_char)
		{
			/* this line in the buffer does not have a valid
			character, free the line and set the pointer te NULL */
			free(inbuf[i]);
			inbuf[i] = NULL;
		}
		else
			break;
	} /* end of for */
}

int appendppbuf(char **appendee, int appendee_cnt, char **appender, int appender_cnt)
{
	int i=0,j=0;
	
	if (!appendee && !appender)
	{
		fprintf(stderr,"appendppbuf: FATAL - ppbuf arguments may not be NULL!\n");	
		return(0);
	}
	
	if (appendee_cnt)
	{
		/* appendee_cnt is the end of the buffer */
		j=appendee_cnt;
		if (appender_cnt)
		{
			/* appender cnt is the length of buffer to append */
			for (i=0; i < appender_cnt; i++,j++)
				appendee[j] = strdup(appender[i]);
		}
		else
		{
			/* append the appender until you hit a NULL */
			for (i=0; appender[i]; i++,j++)
				appendee[j] = strdup(appender[i]);
		}
	}
	else
	{
		/* we must find te end of the appendee buffer */
		for (j=0; appendee[j]; j++)
			;
		
		if (appender_cnt)
		{
			/* appender cnt is the length of buffer to append */
			for (i=0; i < appender_cnt; i++,j++)
				appendee[j] = strdup(appender[i]);
		}
		else
		{
			/* append the appender until you hit a NULL */
			for (i=0; appender[i]; i++,j++)
				appendee[j] = strdup(appender[i]);
		}
	}
	
	return(1);
}

void swap(char **a, char **b)
{
	char *temp;
	
	temp = *a; *a = *b; *b = temp;
}

int split(char **buf, int first, int last)
{
	char *x;
	int unknown;
	int splitPoint;
	
	x = buf[first];
	splitPoint = first;
	
	for (unknown = first + 1; unknown <= last; unknown++)
	{
		if ( (strcmp(buf[unknown],x)) < 0)
		{
			splitPoint++;
			swap(&(buf[splitPoint]), &(buf[unknown]));
		}
	}
	
	swap (&(buf[first]), &(buf[splitPoint]));
	
	return(splitPoint);
}

int Quicksort(char **buf, int buf_cnt)
{
	ppstackp qstack=NULL;
	
	int first, last, splitPoint;
	
	char **sargs, *lasts, *splits;
	
	static char dup_buf[100];
	
	qstack = init_ppstack();
	if (!qstack)
	{
		fprintf(stderr,"Quicksort: FATAL - could not initialize stack!\n");
		return(0);
	}
	
	first = 0;
	last = buf_cnt - 1;
	
	sprintf(dup_buf,"%d",last);
	lasts = strdup(dup_buf);
	mpush(qstack,lasts,"0",NULL);
	if (lasts) free(lasts);
	
	while (qstack->stack_qty > 0)
	{
		sargs = mpop(qstack,2);
		first = atoi(sargs[0]);
		last = atoi(sargs[1]);
		sprintf(dup_buf,"%d",last);
		lasts = strdup(dup_buf);
		free_ppbuf(sargs,0);
		while (first < last)
		{
			splitPoint = split(buf,first,last);
			sprintf(dup_buf,"%d",splitPoint+1);
			splits = strdup(dup_buf);
			mpush(qstack,lasts,splits,NULL);
			if (lasts) free(lasts);
			if (splits) free(splits);
			last = splitPoint - 1;
			sprintf(dup_buf,"%d",last);
			lasts = strdup(dup_buf);
		}
		if (lasts) free(lasts);
	}
	
	return(1);
}

int sortppbuf(char **inbuf, int inbuf_cnt)
{
	/* This function will implement a modified quick sort technique.
	The algorithm is quick sort with a stack from Baase, pg. 61. */
	
	int i=0;
	
	if (!inbuf)
	{
		fprintf(stderr,"sortppbuf: FATAL - argument may not be NULL!\n");
		return(0);
	}
	
	if (!inbuf_cnt)
	{
		for (i=0; inbuf[i]; i++);
		inbuf_cnt = i;
	}
	
	if (!Quicksort(inbuf,inbuf_cnt))
	{
		fprintf(stderr,"sortppbuf: FATAL - Unable to quicksort the ppbuf!\n");
		return(0);
	}
	
	return(1);
}