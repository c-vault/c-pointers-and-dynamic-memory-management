/* This will be a simple C lexical analyzer.  The "lex" utility seems too generic to be
   easily fit to this problem.  It is interesting to note that the GNU compiler does not use
   the "lex" utility. The method this procedure will use is "gobble, flag (type) and analyze."
   The data this procedure will recognize is:
   1) alphanumeric -
      variables
      data
      functions
      keywords (looping, conditional, data storage, math, I/O)
      data types
      compiler directives
   2) punctuation -
      (if characters buffered - evaluate)
      parentheses
      braces
      colon
      semi-colon
      mathematical operators
      relational operators
      bitwise operators
      structure operators
      brackets
      period
      double quotes
      single quotes
      pound sign
      comma
      comments
   3) white space -
      (if characters buffered - evaluate)
      otherwise ignore */

/* *** in order to recognize functions the code would wait until a punction mark
   to analyze a variable (it would check for keywords, data types and type modifiers
   at a space).  Unfortunately, typedefs give us a situation where we have
   variable (space) variable.  This means we cannot just ignore space until we hit a punction
   mark or a variable (space) variable would be concatenated together. To allow this
   analysis will add to more flags - Punct and Space and we will change how the flags are
   interpreted.  If a flag is 0, that still means off; however, a non-zero will represent
   the order of arrival instead of just on. *** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simple_c.h"

#include "memory.h"

/* #define DEBUGLEX */
/* #define DEBUG_KW_IDX */

char *type_str(int type)
{
   switch (type)
   {
     case 1:
	return(strdup("KEYWORD BREAK"));
     case 2:
	return(strdup("KEYWORD ELSE"));
     case 3:
	return(strdup("KEYWORD SWITCH"));
     case 4:
	return(strdup("KEYWORD CASE"));
     case 5:
	return(strdup("KEYWORD TYPEDEF"));
     case 6:
	return(strdup("KEYWORD RETURN"));
     case 7:
	return(strdup("KEYWORD CONTINUE"));
     case 8:
	return(strdup("KEYWORD FOR"));
     case 9:
	return(strdup("KEYWORD DEFAULT"));
     case 10:
	return(strdup("KEYWORD GOTO"));
     case 11:
	return(strdup("KEYWORD SIZEOF"));
     case 12:
	return(strdup("KEYWORD DO"));
     case 13:
	return(strdup("KEYWORD IF"));
     case 14:
	return(strdup("KEYWORD WHILE"));
     case 101:
	return(strdup("DATA TYPE CHAR"));
     case 102:
	return(strdup("DATA TYPE INT"));
     case 103:
	return(strdup("DATA TYPE FLOAT"));
     case 104:
	return(strdup("DATA TYPE DOUBLE"));
     case 105:
	return(strdup("DATA TYPE VOID"));
     case 106:
	return(strdup("DATA TYPE STRUCT"));
     case 107:
	return(strdup("DATA TYPE UNION"));
     case 108:
	return(strdup("DATA TYPE ENUM"));
     case 109:
	return(strdup("DATA TYPE FILE"));
     case 201:
	return(strdup("TYPE MODIFIER SIGNED"));
     case 202:
	return(strdup("TYPE MODIFIER UNSIGNED"));
     case 203:
	return(strdup("TYPE MODIFIER SHORT"));
     case 204:
	return(strdup("TYPE MODIFIER LONG"));
     case 205:
	return(strdup("TYPE MODIFIER EXTERN"));
     case 206:
	return(strdup("TYPE MODIFIER AUTO"));
     case 207:
	return(strdup("TYPE MODIFIER REGISTER"));
     case 208:
	return(strdup("TYPE MODIFIER CONST"));
     case 209:
	return(strdup("TYPE MODIFIER VOLATILE"));
     case 210:
	return(strdup("TYPE MODIFIER STATIC"));
     case 301:
	return(strdup("COMPILER DIRECTIVE DEFINE"));
     case 302:
	return(strdup("COMPILER DIRECTIVE ERROR"));
     case 303:
	return(strdup("COMPILER DIRECTIVE INCLUDE"));
     case 304:
	return(strdup("COMPILER DIRECTIVE IF"));
     case 305:
	return(strdup("COMPILER DIRECTIVE IFDEF"));
     case 306:
	return(strdup("COMPILER DIRECTIVE IFNDEF"));
     case 307:
	return(strdup("COMPILER DIRECTIVE ELSE"));
     case 308:
	return(strdup("COMPILER DIRECTIVE ELIF"));
     case 309:
	return(strdup("COMPILER DIRECTIVE ENDIF"));
     case 1000:
	return(strdup("COMMENT"));
     case 1001:
	return(strdup("QUOTE"));
     case 1002:
	return(strdup("FUNCTION"));
     case 1003:
	return(strdup("END_BLOCK"));
     case 1004:
	return(strdup("BEGIN_BLOCK"));
     case 1005:
	return(strdup("VARIABLE"));
     case 1006:
	/* statement end. */
	return(NULL);
     case 1007:
	return(strdup("POINTER_VARIABLE"));
     case 1008:
	return(strdup("DATA"));
     case 1009:
	return(strdup("ELLIPSE"));
     case 1010:
	return(strdup("ARRAY"));
     case 1011:
	return(strdup("INCLUDE_FILE"));
     case 1012:
	return(strdup("DEFINED_WORD"));
     case 1013:
	return(strdup("FUNCTION_PTR"));
     case 1014:
	return(strdup("OPEN_PARENTHESES"));
     case 1015:
	return(strdup("CLOSE_PARENTHESES"));
     case 1016:
	return(strdup("ARRAY_DIM_VARIABLE"));
     case 1017:
	return(strdup("POINTER_ARRAY_VARIABLE"));
     case 1018:
	return(strdup("COMMA"));
     case 1019:
	return(strdup("EQUAL_SIGN"));
     case 1020:
	return(strdup("DEF_SUBSTITUTION"));
     default:
	return(NULL);
   }
}

int calculate_index(char *in_token, char *inbuf, char *inptr, short int *charcnt_array, int index_offset)
{
   int kw_tot=0,i=0;
   int idx=0,out_type=0;
   short int keyword_length=0;
   short int token_length=0;
   
#ifdef DEBUG_KW_IDX
   printf("As pointers, inbuf =  %p, inptr = %p\n",inbuf,inptr);
   printf("As integers, inbuf =  %d, inptr = %d\n",inbuf,inptr);
#endif   

   /* calculate index of cptr into the keyword string. */
   idx = (int) abs( (int) (inbuf - inptr) );

#ifdef DEBUG_KW_IDX
   printf("idx is %d\n",idx);
#endif   

   for (i = 0; kw_tot <= idx; i++)
      kw_tot += (charcnt_array[i] + 1);

   /* check if this token is actually the keyword or just a "piece" of one */
   token_length = strlen(in_token);
   keyword_length = charcnt_array[i - 1];

   if (token_length == keyword_length)
      out_type = i + index_offset;
   else
      out_type = VARIABLE;
   
   return(out_type);
}

int add2buffer(char **buffer, char Char, int *bufcnt, int *buffer_size, short int comment_flag)
{
   /* This routine will add a character to the token buffer.
      If the buffer needs expansion, it will be expanded. */
   if (*bufcnt == *buffer_size)
   {
      /* expand the buffer */
      if (comment_flag)
      {
	 /* a comment token, expand the buffer by COMMENT_SIZE */
	 *buffer = (char *) realloc(*buffer, *buffer_size + COMMENT_SIZE + 1);
	 if (!*buffer)
	 {
	    fprintf(stderr,"add2buffer: FATAL - malloc failed!\n");
	    return(0);
	 }

	 (*buffer_size) += COMMENT_SIZE + 1;
      }
      else
      {
	 /* a regular token, expand the buffer by TOKSIZE */
	 *buffer = (char *) realloc(*buffer, *buffer_size + TOKSIZE + 1);
	 if (!*buffer)
	 {
	    fprintf(stderr,"add2buffer: FATAL - malloc failed!\n");
	    return(0);
	 }

	 (*buffer_size) += TOKSIZE + 1;
      } /* end of if comment flag */
   } /* end of if need buffer expansion */

   (*buffer)[*bufcnt] = Char;
   (*bufcnt)++;
   (*buffer)[*bufcnt] = '\0';
      
   return(1);
}

short int flag_order(short int type_flag, short int alpha_flag, short int digit_flag,
		     short int punct_flag, short int space_flag)
{
   short int out_order=0;
   short int highest=0;
   
   /* find the highest number flag and set this one higher */
   if ( (alpha_flag > highest) && (type_flag != ALPHA) ) highest = alpha_flag;
   if ( (digit_flag > highest) && (type_flag != DIGIT) ) highest = digit_flag;
   if ( (punct_flag > highest) && (type_flag != PUNCT) ) highest = punct_flag;
   if ( (space_flag > highest) && (type_flag != SPACE) ) highest = space_flag;

   switch (type_flag) {
   case ALPHA:
      if (alpha_flag > highest)
	 out_order = alpha_flag;
      else
	 out_order = highest + 1;
      break;
   case DIGIT:
      if (digit_flag > highest)
	 out_order = digit_flag;
      else
	  out_order = highest + 1;
      break;
   case PUNCT:
      if (punct_flag > highest)
	 out_order = punct_flag;
      else
	  out_order = highest + 1;
      break;
   case SPACE:
      if (space_flag > highest)
	  out_order = space_flag;
      else
	  out_order = highest + 1;
      break;
   }
   
   return(out_order);
}
   
/* get_c_token will return a text token and a token type. */
char *get_c_token(FILE *fp, int *token_type, int last_token)
{
   char ch, *out_buf=NULL;
   char *cptr=NULL;
   
   int cnt=0, buf_size=20;
   int out_type = -1;
   
   /* flags */
   short int characters_buffered=0,done=0,Alpha=0,Digit=0;
   short int Punct=0, Space=0;
   short int Singlequote=0, Poundsign=0;
   short int Commentstart=0, Commentend=0;
   short int Includestart=0, Includeend=0;
   short int Backslash=0, Asterisk=0, Startquote=0, Endquote=0;
   short int Definestart=0, Extendline=0;
   
   /* counters */
   short int dots=0;
   
   /* if end of file, fall to end of routine and return NULL */
   if (!feof(fp))
   {
      /* realloc in chunks of 20 normally, chunks of 80 for comments. */
      out_buf = (char *) malloc(sizeof(char) * (TOKSIZE + 1));
      if (!out_buf)
      {
	 fprintf(stderr,"get_c_token: FATAL - malloc failed!\n");
	 return(NULL);
      }
   }

   /* loop, reading characters, until end-of-file */
   while ( (!done) && ((ch = getc(fp)) != EOF) )
   {
      /* determine the type of character received */
      if (isalpha(ch))
      {
	 /* if not inside a comment or quote or a #defined substitution */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (last_token != CD_DEFINE) )
	 {
	    /* to evaluate variable (space) variable we need
	       to check the order of the space flag */
	    if (characters_buffered)
	    {
	       /* if the space flag is greater than one
		  than there is space between two variables
		  (probably a typedef) */
	       if (Space > 1)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {	       
		  /* letters - store in buffer and set flag.  */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  
		  if (!Alpha)
		     Alpha = flag_order(ALPHA,Alpha,Digit,Punct,Space);
		  
		  characters_buffered = 1;
	       }
	    }
	    else
	    {
	       /* letters - store in buffer and set flag.  */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       if (!Alpha)
		  Alpha = flag_order(ALPHA,Alpha,Digit,Punct,Space);
	       
	       characters_buffered = 1;
	    }
	 } /* in comment or quote or def_substitution */
	 else
	 {
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }
	    
	    characters_buffered = 1;
	    if (last_token == CD_DEFINE)
	       Definestart=1;
	    
	 } /* end of if Commentend */	    
      } /* end of isalpha */
      else if (isdigit(ch))
      {
	 /* to evaluate variable (space) variable we need
	    to check the order of the space flag */
	 /* if not inside a comment or quote or define substitution */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) && (!Definestart) )
	 {
	    if (characters_buffered)
	    {
	       /* if the space flag is greater than one
		  than there is space between two variables
		  (probably a typedef) */
	       if (Space > 1)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {	       
		  /* digits - store in buffer and set flag.  */
		  if (!Digit)
		     Digit = flag_order(DIGIT,Alpha,Digit,Punct,Space);
		  
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  
		  Digit = 1;
		  characters_buffered = 1;
	       }
	    }
	    else
	    {	       
	       /* digits - store in buffer and set flag.  */
	       if (!Digit)
		  Digit = flag_order(DIGIT,Alpha,Digit,Punct,Space);
	       
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       Digit = 1;
	       characters_buffered = 1;
	    }
	 }
	 else
	 {
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }
	    
	    characters_buffered = 1;
	 } /* end of if Commentend */	    
      }
      else if (ispunct(ch))
      {
	 /* if not inside a comment or quote */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) )
	 {

	    /* since everything is evaluated at punctuation, do not
	       store the punctuation order */

	    /* switch on the possible punctuation types */
	    switch (ch) {
	    case '[':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_ARRAY_VARIABLE;
		  else
		     out_type = ARRAY;
	       }
	       break;
	    case ']':
	       if (characters_buffered)
	       {
		  done = 1;
		  out_type = ARRAY_DIM_VARIABLE;
	       }
	       break;
	    case '\'':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  Singlequote = 1;
	       }
	       break;
	    case ')':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  /* check for keywords, i.e. void all by itself */
		  if (strlen(out_buf) > 1)
		  {
		     /* check if data type */
		     if ( (cptr = strstr(data_types,out_buf)) != NULL)
		     {
			/* a data type, calculate an index */
			out_type = calculate_index(out_buf,data_types,cptr,dt_length,DATA_TYPE_OFFSET);
		     }
		     else
		     {
			/* not a keyword, some type of variable */
			if (Asterisk)
			   out_type = POINTER_VARIABLE;
			else if ( (Digit) && (!Alpha) )
			   out_type = DATA;
			else
			   out_type = VARIABLE;
		     }
		  } /* out_buf > 1 character */
		  else
		  {
		     if (Asterisk)
			out_type = POINTER_VARIABLE;
		     else if ( (Digit) && (!Alpha) )
			out_type = DATA;
		     else
			out_type = VARIABLE;
		  }
	       }
	       else
	       {
		  /* add to buffer as CLOSE_PARENTHESES token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = CLOSE_PARENTHESES;		  
	       }
	       break;
	    case '/':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* either divide or comment */
		  Backslash = 1;
	       }
	       break;
	    case '+':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '-':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '=':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* add to buffer as EQUAL_SIGN token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = EQUAL_SIGN;		  
	       }
	       break;
	    case '?':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '%':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '*':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* either an operator, a Commentstart, or a pointer */
		  Asterisk = 1;
		  if (Backslash)
		  {
		     Commentstart = 1;
		     out_type = COMMENT;
#ifdef DEBUGLEX
		     printf("Starting comment.\n");
#endif		  
		  }		     
	       }
	       break;
	    case '<':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator or include file */
		  if (last_token == CD_INCLUDE)
		     Includestart = 1;
		  out_type = INCLUDE_FILE;
	       }
	       break;
	    case '>':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '!':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '|':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '&':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '~':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* bitwise operator */

	       }
	       break;
	    case '^':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '\"':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  if (!Singlequote)
		  {
#ifdef DEBUGLEX
		     printf("Starting quote.\n");
#endif		  
		     /* Startquote */
		     Startquote = 1;
		     out_type = QUOTE;
		  }
	       }
	       break;
	    case ',':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       } /* else pass out the comma */
	       else
	       {
		  /* add to buffer as COMMA token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = COMMA;		  
	       }
	       break;
	    case '(':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword or function or function ptr */
		     if ( (cptr = strstr(keywords,out_buf)) == NULL)
		     {
			/* not a keyword, must be a function */
			out_type = FUNCTION;
		     }
		     else
		     {
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
		     }
		  } /* out_buf > 1 character */
	       }
	       else
	       {
		  /* add to buffer as OPEN_PARENTHESES token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = OPEN_PARENTHESES;		  
	       }
	       break;
	    case ':':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword */
		     if ( (cptr = strstr(keywords,out_buf)) != NULL)
		     {
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
		     }
		  }
		  /* else ignore */
	       }
	       break;
	    case ';':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else if ( !(strcmp(out_buf,"return")))
		     out_type = KW_RETURN;
		  else
		     out_type = VARIABLE;
	       }		  
	       else
	       {
		  /* add to buffer as STATEMENT_END token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = STATEMENT_END;		  
	       }
	       /* else ignore */
	       break;
	    case '{':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* add to buffer as BEGIN BLOCK token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = BEGIN_BLOCK;		  
	       }
	       /* else ignore */
	       break;
	    case '}': /* END BLOCK */
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
		  ungetc(ch,fp);
	       }
	       else
	       {
		  /* add to buffer as END BLOCK token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = END_BLOCK;
	       }
	       break;
	    case '#': /* COMPILER DIRECTIVE */
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
		  ungetc(ch,fp);
	       }
	       else
	       {
		  /* set the flag */
		  Poundsign = 1;
	       }
	       break;
	    case '_':
	       /* this is a valid variable character */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       break;
	    case '.':
	       /* either a decimal point, ellipse, or dot operator */
	       dots++;
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       if (dots == 3)
		  out_type = ELLIPSE;
	       break;
	    default:
	       printf("unknown punctuation!\n");
	    }; /* end of switch */
	 }
	 else
	 {
	    /* buffer all comment and quote characters */
	    /* first check for comment, quote or include end marker */
	    if ( (Commentstart) && (ch == '/') && (out_buf[cnt-1] == '*') )
	    {
	       Commentstart = 0;
	       Commentend = 1;
	       done = 1;
	    }
	    else if ( (Startquote) && (ch == '"')  )
	    {
	       Startquote = 0;
	       Endquote = 1;
	       done = 1;
	    }
	    else if ( (Includestart) && (ch == '>')  )
	    {
	       Includestart = 0;
	       Includeend = 1;
	       done = 1;
	    }
	    else if ( (Definestart) && (ch == '\\') )
	    {
	       Extendline = 1;
	    }
	    else
	    {
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }

	       characters_buffered = 1;
	    } /* end of if Commentend */	    
	 } /* end of if Commentstart */  
      }
      else if (isspace(ch))
      {
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) )
	 {
	    Space = flag_order(SPACE,Alpha,Digit,Punct,Space);

	    if (characters_buffered)
	    {
	       /* check if a defined word */
	       if ( (last_token == CD_IFDEF) || (last_token == CD_IFNDEF) )
	       {
		  done = 1;
		  out_type = DEFINED_WORD;
	       }
	       else
	       {
		  /* determine type */
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword */
		     if ( (cptr = strstr(keywords,out_buf)) != NULL)
		     {
			/* a keyword, calculate the index */
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
			done = 1;
		     }
		     else if ( (cptr = strstr(data_types,out_buf)) != NULL)
		     {
			/* a data type, calculate an index */
			out_type = calculate_index(out_buf,data_types,cptr,dt_length,DATA_TYPE_OFFSET);
			done = 1;
		     }
		     else if ( (cptr = strstr(type_modifiers,out_buf)) != NULL)
		     {
			/* a type modifier, calculate an index */
			out_type = calculate_index(out_buf,type_modifiers,cptr,tm_length,TYPE_MODIFIER_OFFSET);
			done = 1;
		     }
		     else if (Poundsign)
		     {
			/* check if a compiler directive */
			if ( (cptr = strstr(compiler_directives,out_buf)) != NULL)
			{
			   /* a compiler directive, calculate an index */
			   out_type = calculate_index(out_buf,compiler_directives,cptr,cd_length,COMPILER_DIRECTIVE_OFFSET);
			   done = 1;
			}
		     }
		  }
	       }
	       /* else ignore */
	    }
	    /* else ignore */
	 }
	 else if ( (Definestart) && (ch == '\n') )
	 {
	    if (Extendline)
	    {
	       Extendline = 0;
	       /* buffer all comment characters */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       characters_buffered = 1;
	    }
	    else
	    {
	       Definestart = 0;
	       done = 1;
	       out_type = DEF_SUBSTITUTION;
	    }
	 }
	 else
	 {
	    /* buffer all comment characters */
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }

	    characters_buffered = 1;
	 }
      }
      else
      {
	 /* error, unknown character */
	 fprintf(stderr,"get_c_token: FATAL - Unknown character, in decimal <%d>\n",ch);
	 return(NULL);
      } /* end of if else ladder */
   } /* end of while loop */

   *token_type = out_type;
   return(out_buf);
}


/* *** this function is almost identical to get_c_token, except that it has the
   added functionality of file-byte tagging. *** */
char *ctok_n_fpos(FILE *fp, int *token_type, int last_token, long *file_pos)
{
   char ch, *out_buf=NULL;
   char *cptr=NULL;
   
   int cnt=0, buf_size=20;
   int out_type = -1;
   
   /* flags */
   short int characters_buffered=0,done=0,Alpha=0,Digit=0;
   short int Punct=0, Space=0;
   short int Singlequote=0, Poundsign=0;
   short int Commentstart=0, Commentend=0;
   short int Includestart=0, Includeend=0;
   short int Backslash=0, Asterisk=0, Startquote=0, Endquote=0;
   short int Definestart=0, Extendline=0;
   short int store_fpos=0;
   
   /* counters */
   short int dots=0;
   
   /* if end of file, fall to end of routine and return NULL */
   if (!feof(fp))
   {
      /* realloc in chunks of 20 normally, chunks of 80 for comments. */
      out_buf = (char *) malloc(sizeof(char) * (TOKSIZE + 1));
      if (!out_buf)
      {
	 fprintf(stderr,"get_c_token: FATAL - malloc failed!\n");
	 return(NULL);
      }
   }

   /* loop, reading characters, until end-of-file */
   while ( (!done) && ((ch = getc(fp)) != EOF) )
   {
      /* As soon as one character is buffered, store the file position */
      if (characters_buffered && !store_fpos)
      {
	 store_fpos = 1;
	 *file_pos = ftell(fp);
      }

      /* determine the type of character received */
      if (isalpha(ch))
      {
	 /* if not inside a comment or quote or a #defined substitution */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (last_token != CD_DEFINE) )
	 {
	    /* to evaluate variable (space) variable we need
	       to check the order of the space flag */
	    if (characters_buffered)
	    {
	       /* if the space flag is greater than one
		  than there is space between two variables
		  (probably a typedef) */
	       if (Space > 1)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {	       
		  /* letters - store in buffer and set flag.  */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  
		  if (!Alpha)
		     Alpha = flag_order(ALPHA,Alpha,Digit,Punct,Space);
		  
		  characters_buffered = 1;
	       }
	    }
	    else
	    {
	       /* letters - store in buffer and set flag.  */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       if (!Alpha)
		  Alpha = flag_order(ALPHA,Alpha,Digit,Punct,Space);
	       
	       characters_buffered = 1;
	    }
	 } /* in comment or quote or def_substitution */
	 else
	 {
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }
	    
	    characters_buffered = 1;
	    if (last_token == CD_DEFINE)
	       Definestart=1;
	    
	 } /* end of if Commentend */	    
      } /* end of isalpha */
      else if (isdigit(ch))
      {
	 /* to evaluate variable (space) variable we need
	    to check the order of the space flag */
	 /* if not inside a comment or quote or define substitution */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) && (!Definestart) )
	 {
	    if (characters_buffered)
	    {
	       /* if the space flag is greater than one
		  than there is space between two variables
		  (probably a typedef) */
	       if (Space > 1)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {	       
		  /* digits - store in buffer and set flag.  */
		  if (!Digit)
		     Digit = flag_order(DIGIT,Alpha,Digit,Punct,Space);
		  
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  
		  Digit = 1;
		  characters_buffered = 1;
	       }
	    }
	    else
	    {	       
	       /* digits - store in buffer and set flag.  */
	       if (!Digit)
		  Digit = flag_order(DIGIT,Alpha,Digit,Punct,Space);
	       
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       Digit = 1;
	       characters_buffered = 1;
	    }
	 }
	 else
	 {
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }
	    
	    characters_buffered = 1;
	 } /* end of if Commentend */	    
      }
      else if (ispunct(ch))
      {
	 /* if not inside a comment or quote */
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) )
	 {

	    /* since everything is evaluated at punctuation, do not
	       store the punctuation order */

	    /* switch on the possible punctuation types */
	    switch (ch) {
	    case '[':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_ARRAY_VARIABLE;
		  else
		     out_type = ARRAY;
	       }
	       break;
	    case ']':
	       if (characters_buffered)
	       {
		  done = 1;
		  out_type = ARRAY_DIM_VARIABLE;
	       }
	       break;
	    case '\'':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  Singlequote = 1;
	       }
	       break;
	    case ')':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  /* check for keywords, i.e. void all by itself */
		  if (strlen(out_buf) > 1)
		  {
		     /* check if data type */
		     if ( (cptr = strstr(data_types,out_buf)) != NULL)
		     {
			/* a data type, calculate an index */
			out_type = calculate_index(out_buf,data_types,cptr,dt_length,DATA_TYPE_OFFSET);
		     }
		     else
		     {
			/* not a keyword, some type of variable */
			if (Asterisk)
			   out_type = POINTER_VARIABLE;
			else if ( (Digit) && (!Alpha) )
			   out_type = DATA;
			else
			   out_type = VARIABLE;
		     }
		  } /* out_buf > 1 character */
		  else
		  {
		     if (Asterisk)
			out_type = POINTER_VARIABLE;
		     else if ( (Digit) && (!Alpha) )
			out_type = DATA;
		     else
			out_type = VARIABLE;
		  }
	       }
	       else
	       {
		  /* add to buffer as CLOSE_PARENTHESES token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = CLOSE_PARENTHESES;		  
	       }
	       break;
	    case '/':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* either divide or comment */
		  Backslash = 1;
	       }
	       break;
	    case '+':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '-':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '=':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* add to buffer as EQUAL_SIGN token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = EQUAL_SIGN;		  
	       }
	       break;
	    case '?':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '%':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* mathematical operator */

	       }
	       break;
	    case '*':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* either an operator, a Commentstart, or a pointer */
		  Asterisk = 1;
		  if (Backslash)
		  {
		     Commentstart = 1;
		     out_type = COMMENT;
#ifdef DEBUGLEX
		     printf("Starting comment.\n");
#endif		  
		  }		     
	       }
	       break;
	    case '<':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator or include file */
		  if (last_token == CD_INCLUDE)
		     Includestart = 1;
		  out_type = INCLUDE_FILE;
	       }
	       break;
	    case '>':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '!':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '|':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '&':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '~':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* bitwise operator */

	       }
	       break;
	    case '^':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* relational operator */

	       }
	       break;
	    case '\"':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  if (!Singlequote)
		  {
#ifdef DEBUGLEX
		     printf("Starting quote.\n");
#endif		  
		     /* Startquote */
		     Startquote = 1;
		     out_type = QUOTE;
		  }
	       }
	       break;
	    case ',':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       } /* else pass out the comma */
	       else
	       {
		  /* add to buffer as COMMA token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = COMMA;		  
	       }
	       break;
	    case '(':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword or function or function ptr */
		     if ( (cptr = strstr(keywords,out_buf)) == NULL)
		     {
			/* not a keyword, must be a function */
			out_type = FUNCTION;
		     }
		     else
		     {
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
		     }
		  } /* out_buf > 1 character */
	       }
	       else
	       {
		  /* add to buffer as OPEN_PARENTHESES token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = OPEN_PARENTHESES;		  
	       }
	       break;
	    case ':':
	       if (characters_buffered)
	       {
		  done = 1;
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword */
		     if ( (cptr = strstr(keywords,out_buf)) != NULL)
		     {
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
		     }
		  }
		  /* else ignore */
	       }
	       break;
	    case ';':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else if ( !(strcmp(out_buf,"return")))
		     out_type = KW_RETURN;
		  else
		     out_type = VARIABLE;
	       }		  
	       else
	       {
		  /* add to buffer as STATEMENT_END token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = STATEMENT_END;		  
	       }
	       /* else ignore */
	       break;
	    case '{':
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
	       }
	       else
	       {
		  /* add to buffer as BEGIN BLOCK token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = BEGIN_BLOCK;		  
	       }
	       /* else ignore */
	       break;
	    case '}': /* END BLOCK */
	       if (characters_buffered)
	       {
		  done = 1;
		  ungetc(ch,fp);
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
		  ungetc(ch,fp);
	       }
	       else
	       {
		  /* add to buffer as END BLOCK token */
		  if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
		  {
		     fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		     return(NULL);
		  }
		  done = 1;
		  out_type = END_BLOCK;
	       }
	       break;
	    case '#': /* COMPILER DIRECTIVE */
	       if (characters_buffered)
	       {
		  done = 1;
		  if (Asterisk)
		     out_type = POINTER_VARIABLE;
		  else if ( (Digit) && (!Alpha) )
		     out_type = DATA;
		  else
		     out_type = VARIABLE;
		  ungetc(ch,fp);
	       }
	       else
	       {
		  /* set the flag */
		  Poundsign = 1;
	       }
	       break;
	    case '_':
	       /* this is a valid variable character */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       break;
	    case '.':
	       /* either a decimal point, ellipse, or dot operator */
	       dots++;
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       if (dots == 3)
		  out_type = ELLIPSE;
	       break;
	    default:
	       printf("unknown punctuation!\n");
	    } /* end of switch */
	 }
	 else
	 {
	    /* buffer all comment and quote characters */
	    /* first check for comment, quote or include end marker */
	    if ( (Commentstart) && (ch == '/') && (out_buf[cnt-1] == '*') )
	    {
	       Commentstart = 0;
	       Commentend = 1;
	       done = 1;
	    }
	    else if ( (Startquote) && (ch == '"')  )
	    {
	       Startquote = 0;
	       Endquote = 1;
	       done = 1;
	    }
	    else if ( (Includestart) && (ch == '>')  )
	    {
	       Includestart = 0;
	       Includeend = 1;
	       done = 1;
	    }
	    else if ( (Definestart) && (ch == '\\') )
	    {
	       Extendline = 1;
	    }
	    else
	    {
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }

	       characters_buffered = 1;
	    } /* end of if Commentend */	    
	 } /* end of if Commentstart */  
      }
      else if (isspace(ch))
      {
	 if ( (!Commentstart) && (!Startquote) && (!Includestart) && (!Definestart) )
	 {
	    Space = flag_order(SPACE,Alpha,Digit,Punct,Space);

	    if (characters_buffered)
	    {
	       /* check if a defined word */
	       if ( (last_token == CD_IFDEF) || (last_token == CD_IFNDEF) )
	       {
		  done = 1;
		  out_type = DEFINED_WORD;
	       }
	       else
	       {
		  /* determine type */
		  if (strlen(out_buf) > 1)
		  {
		     /* check if keyword */
		     if ( (cptr = strstr(keywords,out_buf)) != NULL)
		     {
			/* a keyword, calculate the index */
			/* determine which keyword */
			out_type = calculate_index(out_buf,keywords,cptr,kw_length,KEY_WORD_OFFSET);
			done = 1;
		     }
		     else if ( (cptr = strstr(data_types,out_buf)) != NULL)
		     {
			/* a data type, calculate an index */
			out_type = calculate_index(out_buf,data_types,cptr,dt_length,DATA_TYPE_OFFSET);
			done = 1;
		     }
		     else if ( (cptr = strstr(type_modifiers,out_buf)) != NULL)
		     {
			/* a type modifier, calculate an index */
			out_type = calculate_index(out_buf,type_modifiers,cptr,tm_length,TYPE_MODIFIER_OFFSET);
			done = 1;
		     }
		     else if (Poundsign)
		     {
			/* check if a compiler directive */
			if ( (cptr = strstr(compiler_directives,out_buf)) != NULL)
			{
			   /* a compiler directive, calculate an index */
			   out_type = calculate_index(out_buf,compiler_directives,cptr,cd_length,COMPILER_DIRECTIVE_OFFSET);
			   done = 1;
			}
		     }
		  }
	       }
	       /* else ignore */
	    }
	    /* else ignore */
	 }
	 else if ( (Definestart) && (ch == '\n') )
	 {
	    if (Extendline)
	    {
	       Extendline = 0;
	       /* buffer all comment characters */
	       if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	       {
		  fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
		  return(NULL);
	       }
	       
	       characters_buffered = 1;
	    }
	    else
	    {
	       Definestart = 0;
	       done = 1;
	       out_type = DEF_SUBSTITUTION;
	    }
	 }
	 else
	 {
	    /* buffer all comment characters */
	    if (!add2buffer(&out_buf,ch,&cnt,&buf_size,Commentstart))
	    {
	       fprintf(stderr,"get_c_token: FATAL - add2buffer failed!\n");
	       return(NULL);
	    }

	    characters_buffered = 1;
	 }
      }
      else
      {
	 /* error, unknown character */
	 fprintf(stderr,"get_c_token: FATAL - Unknown character, in decimal <%d>\n",ch);
	 return(NULL);
      } /* end of if else ladder */
   } /* end of while loop */

   *token_type = out_type;
   return(out_buf);
}
