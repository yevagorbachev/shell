# SHELL
By Yevgeniy Gorbachev and Amanda Chen
## Features
1. Reading one line at a time, parsing the line to separate a command from its arguments
2. Forks and executes the command
    1. The parent process waits until the executed program exits before reading the next command
3. Reads and separates multiple commands on one line with ;
    1. ls -l;echo hello 
    2. First runs the command ls -l and then echo hello
4. cd command traverses through directories
5. Simple redirection with one > or <
    1. w > wholist or tr a-z A-Z < wholist
6. Double redirection with < then >
    1. tr a-z A-Z < wholist > foo
7. Simple piping with one |
    1. ls | wc
8. exit command ends the shell program

## Bugs
1. Only one whitespace at most is allowed between commands and functions ( |, <, >, ; ) or the shell will break
2. Semicolon after command will produce an errno
3. Control-C will exit shell even if there is a function currently running

## Files & Function Headers
### parsing.c
Handles all line parsing functions
```
char * clean_sep(char ** line, char delim); 
```
Returns: Pointer to the beginning of line<br />
Functions similarly to strsep, but overwrites surrounding whitespace (up to 1 character on each side) with nulls as well
```
char ** sep_line(char * line, char * delim);
```
Returns: Array of pointers to each token; array is malloc'd<br />
Separates the null-terminated string pointed to by line using the delimiter delim, using strsep<br />
Supports a maximum of 20 delimiters
```
char ** clean_sep_line(char * line, char delim);
```
Returns: Array of pointers to each token; array is malloc'd<br />
Functions identically to sep_line, but replaces the use of strsep with clean_sep<br />

### shell.c
Handles the forking and execution of commands
```
void redirect_out(char * cmd);
```
Takes the null-terminated string pointed to by cmd<br />
Redirects STDOUT; overwrites existing contents of output file<br />
Uses clean_sep_line to separate by '>'<br />
Frees malloc from clean_sep_line call<br />
```
void redirect_in(char * cmd);
```
Takes the null-terminated string pointed to by cmd<br />
Replaces STDIN with the contents of the input file<br />
Executes the command using exec_single<br />
Uses clean_sep_line to separate by '<'<br />
Frees malloc from clean_sep_line call
```
void redirect_double(char * cmd); //redirects both STDOUT and STDIN
```
Takes the null-terminated string pointed to by cmd<br />
Uses clean_sep_line to separate by '<' then '>'<br />
Frees mallocs from clean_sep_line calls
```
int my_pipe(char ** cmdv);
```
Returns: Exit status of the child process (the full int written by wait())<br />
Takes the null-terminated string pointed to by cmd and evaluates as a single pipe command<br />
Forks<br />
Uses popen() to execute the two commands<br />
Uses clean_sep_line to separate by '|'<br />
Frees malloc from clean_sep_line call<br />
Returns the exit status of the child process (the full int written by wait())
```
void read_command(char * cmdbuffer);
```
Takes a pointer cmdbuffer to a character buffer<br />
Prints a prompt consisting of the current working directory followed by '$' to STDOUT<br />
Uses fgets to read BUFFERSIZE characters from STDIN into the command buffer
```
int exec_single(char * cmd);
```
Returns: Either 0 or the exit status (the full int written by wait()) of the child<br />
Takes the null-terminated string pointed to by cmd and executes as a single command, calling my_pipe, redirect_out, redirect_in, redirect_double as necessary<br />
Separates command (once stripped of pipes and redirectons) by ' ' using sep_line<br />
Forks<br />
Uses execvp to execute command<br />
Frees malloc from sep_line call
```
void exec_all(char * cmds);
```
Makes a null-terminated array of pointers to null-terminated command strings<br />
Runs exec_single for each command string<br />
Uses clean_sep to separate by ';'<br />
Frees the malloc from clean_sep call
