#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


char ** parse_args(char * line); // separates line by " ", returns array, MALLOCs
int read_command(char * cmdbuffer); // fgets to cmdbuffer
int exec_command(char * cmd); // uses parse_args to make cmd an argv, frees malloc from parse_args
