#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "parsing.h"

#define BUFFERSIZE 256 // max number of characters in command buffer
#define CWDSIZE 1024 // max number of characters in path of CWD


void read_command(char * cmdbuffer); // fgets BUFFERSIZE characters, writes to cmdbuffer
int exec_single(char * cmd); // makes cmd an argv through sep_line(," "), executes argv
// MALLOC (from sep_line) + FREE
void exec_all(char * cmds); // makes cmds a cmdv through sep_line(,";"), exec_single each in cmdv
// MALLOC 1 (from sep_line) + FREE 1
// MALLOC 2 (from exec_single) + FREE 2
void redirect_all(char * cmd); //
void redirect_out(char * cmd); //redirects STDOUT to a file and overwrites existing contens
void redirect_in(char * cmd); //redirects STDIN from a file and replaces it
