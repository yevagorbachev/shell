#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


char ** sep_line(char * line, char * delim); // separates line by delim, returns array, MALLOCs
int read_command(char * cmdbuffer); // fgets to cmdbuffer
int exec_command(char * cmd); // uses sep_line to make cmd an argv, frees malloc from sep_line
