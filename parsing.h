#include <string.h>
#include <stdlib.h>

char * clean_sep(char ** line, char delim); // functions like strsep but replaces spaces surrounding the single delimiter with nulls
//SEP_LINEs allocate heap memory
char ** sep_line(char * line, char * delim); // separates line by delim, returns array
char ** clean_sep_line(char * line, char delim); // like sep_line but uses clean_sep instead of strsep, on a single-character delimiter.