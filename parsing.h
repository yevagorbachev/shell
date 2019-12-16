#include <string.h>
#include <stdlib.h>

char * clean_sep(char ** line, char delim);
// Functions similarly to strsep, but overwrites surrounding whitespace (up to 1 character on each side) with nulls as well

char ** sep_line(char * line, char delim);
// separates the null-terminated string pointed to by line using the delimiter delim, using strsep
// supports a maximum of 20 delimiters
// returns an array of pointers to each token; array is malloc'd

char ** clean_sep_line(char * line, char delim);
// functions identically to sep_line, but replaces the use of strsep with clean_sep