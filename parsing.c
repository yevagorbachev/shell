#include "parsing.h"

char ** sep_line(char * line, char * delim) {
    char ** argv = calloc(sizeof(char *), 20);
    int i = 0;
    while ((argv[i++] = strsep(&line, delim)) != NULL) {}
    return argv;
}


char * clean_sep(char ** line, char delim) {
    char * start = *line;
    char * next = *line;
    if ((next != NULL) && ((next = strchr(next, delim)))) {
        *next = '\0';
        if (*(next - 1) == ' ') {
            *(next - 1) = '\0';
        }
        if (*(++next) == ' ') {
            *(next++) = '\0';
        }
    }
    *line = next;
    return start;
}


char ** clean_sep_line(char * line, char delim) {
    char ** argv = calloc(sizeof(char *), 20);
    int i = 0;
    while ((argv[i++] = clean_sep(&line, delim)) != NULL) {}
    return argv;
}
