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


void redirect_out(char * cmd);
// takes the null-terminated string pointed to by cmd
// redirects STDOUT; overwrites existing contents of output file
// uses clean_sep_line to separate by '>'
// frees malloc from clean_sep_line call

void redirect_in(char * cmd);
// takes the null-terminated string pointed to by cmd
// replaces STDIN with the contents of the input file
// executes the command using exec_single
// uses clean_sep_line to separate by '<'
// frees malloc from clean_sep_line call

void redirect_double(char * cmd);
// takes the null-terminated string pointed to by cmd
// uses clean_sep_line to separate by '<' then '>'
// frees mallocs from clean_sep_line calls

int my_pipe(char * cmd);
// takes the null-terminated string pointed to by cmd and evaluates as a single pipe command
// forks
// uses popen() to execute the two commands
// uses clean_sep_line to separate by '|'
// frees malloc from clean_sep_line call
// returns the exit status of the child process (the full int written by wait())

void read_command(char * cmdbuffer);
// takes a pointer cmdbuffer to a character buffer
// prints a prompt consisting of the current working directory followed by '$' to STDOUT
// uses fgets to read BUFFERSIZE characters from STDIN into the command buffer


int exec_single(char * cmd);
// takes the null-terminated string pointed to by cmd and executes as a single command;
//      calling my_pipe, redirect_out, redirect_in, redirect_double as necessary
// separates command (once stripped of pipes and redirectons) by ' ' using sep_line
// forks
// uses execvp to execute command
// frees malloc from sep_line call
// returns either 0 or the exit status (the full int written by wait()) of the child


void exec_all(char * cmds);
// makes a null-terminated array of pointers to null-terminated command strings
// runs exec_single for each command string
// uses clean_sep to separate by ';'
// frees the malloc from clean_sep call
