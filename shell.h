#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERSIZE 256

char ** sep_line(char * line, char * delim); // separates line by delim, returns array
// MALLOC
void read_command(char * cmdbuffer); // fgets BUFFERSIZE characters, writes to cmdbuffer
int exec_single(char * cmd); // makes cmd an argv through sep_line(," "), executes argv 
// MALLOC (from sep_line) + FREE
void exec_all(char * cmds); // makes cmds a cmdv through sep_line(,";"), exec_single each in cmdv
// MALLOC 1 (from sep_line) + FREE 1
// MALLOC 2 (from exec_single) + FREE 2
