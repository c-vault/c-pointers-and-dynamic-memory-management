#define TOKSIZE 20
#define COMMENT_SIZE 80
#define ALPHA 1
#define DIGIT 2
#define PUNCT 3
#define SPACE 4

/* TOKEN TYPES */
#define KEY_WORD_OFFSET 0
#define DATA_TYPE_OFFSET 100
#define TYPE_MODIFIER_OFFSET 200
#define COMPILER_DIRECTIVE_OFFSET 300
#define KW_BREAK 1
#define KW_ELSE 2
#define KW_SWITCH 3
#define KW_CASE 4
#define KW_TYPEDEF 5
#define KW_RETURN 6
#define KW_CONTINUE 7
#define KW_FOR 8
#define KW_DEFAULT 9
#define KW_GOTO 10
#define KW_SIZEOF 11
#define KW_DO 12
#define KW_IF 13
#define KW_WHILE 14
#define DT_CHAR 101
#define DT_INT 102
#define DT_FLOAT 103
#define DT_DOUBLE 104
#define DT_VOID 105
#define DT_STRUCT 106
#define DT_UNION 107
#define DT_ENUM 108
#define DT_FILE 109
#define TM_SIGNED 201
#define TM_UNSIGNED 202
#define TM_SHORT 203
#define TM_LONG 204
#define TM_EXTERN 205
#define TM_AUTO 206
#define TM_REGISTER 207
#define TM_CONST 208
#define TM_VOLATILE 209
#define TM_STATIC 210
#define CD_DEFINE 301
#define CD_ERROR 302
#define CD_INCLUDE 303
#define CD_IF 304
#define CD_IFDEF 305
#define CD_IFNDEF 306
#define CD_ELSE 307
#define CD_ELIF 308
#define CD_ENDIF 309
#define COMMENT 1000
#define QUOTE 1001
#define FUNCTION 1002
#define END_BLOCK 1003
#define BEGIN_BLOCK 1004
#define VARIABLE 1005
#define STATEMENT_END 1006
#define POINTER_VARIABLE 1007
#define DATA 1008
#define ELLIPSE 1009
#define ARRAY 1010
#define INCLUDE_FILE 1011
#define DEFINED_WORD 1012
#define FUNCTION_PTR 1013
#define OPEN_PARENTHESES 1014
#define CLOSE_PARENTHESES 1015
#define ARRAY_DIM_VARIABLE 1016
#define POINTER_ARRAY_VARIABLE 1017
#define COMMA 1018
#define EQUAL_SIGN 1019
#define DEF_SUBSTITUTION 1020

static char *keywords =
   "break|else|switch|case|typedef|return|continue|for|default|goto|sizeof|do|if|while|";

static short int kw_length[14] =
{
   5,4,6,4,7,6,8,3,7,4,6,2,2,5,
};

static char *data_types =
   "char|int|float|double|void|struct|union|enum|FILE|";

static short int dt_length[9] =
{
   4,3,5,6,4,6,5,4,4,
};

static char *type_modifiers =
   "signed|unsigned|short|long|extern|auto|register|const|volatile|static|";

static short int tm_length[10] =
{
   6,8,5,4,6,4,8,5,8,6,
};

static char *compiler_directives =
   "define|error|include|if|ifdef|ifndef|else|elif|endif|";

static short int cd_length[9] =
{
   6,5,7,2,5,6,4,4,5,
};

