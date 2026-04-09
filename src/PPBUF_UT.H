/* ppbuf_utils.h */

#ifndef _PPBUF_UTILS
#define _PPBUF_UTILS

extern char **file2ppbuf(char *filename);
extern int count_lines(char **inbuf);
extern void strip_trailing_whitespace(char **inbuf);
extern void free_ppbuf(char **inbuf, int count);
extern char **expandppbuf(char **inbuf, int numlines);
extern int dumpppbuf(char **ppbuf, char *filename, int numlines, 
int addnewline);
extern char **dupnlines(char **buf, int from, int numlines);
extern char **dupppbuf(char **ppbuf, int count);
extern int appendppbuf(char **appendee, int appendee_cnt, char **appender,
int appender_cnt);
extern int sortppbuf(char **inbuf, int inbuf_cnt);

#endif