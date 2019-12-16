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
Returns: Pointer to the beginning of line
- Functions similarly to strsep, but overwrites surrounding whitespace (up to 1 character on each side) with nulls as well
```
char ** sep_line(char * line, char * delim);
```
Returns: Array of pointers to each token; array is malloc'd
- Separates the null-terminated string pointed to by line using the delimiter delim, using strsep
- Supports a maximum of 20 delimiters
```
char ** clean_sep_line(char * line, char delim);
```
Returns: Array of pointers to each token; array is malloc'd
- Functions identically to sep_line, but replaces the use of strsep with clean_sep

### shell.c
Handles the forking and execution of commands
```
void redirect_out(char * cmd);
```
- Takes the null-terminated string pointed to by cmd
- Redirects STDOUT; overwrites existing contents of output file
- Uses clean_sep_line to separate by '>'
- Frees malloc from clean_sep_line call
```
void redirect_in(char * cmd);
```
- Takes the null-terminated string pointed to by cmd
- Replaces STDIN with the contents of the input file
- Executes the command using exec_single
- Uses clean_sep_line to separate by '<'
- Frees malloc from clean_sep_line call
```
void redirect_double(char * cmd); //redirects both STDOUT and STDIN
```
- Takes the null-terminated string pointed to by cmd
- Uses clean_sep_line to separate by '<' then '>'
- Frees mallocs from clean_sep_line calls
```
int my_pipe(char ** cmdv);
```
Returns: Exit status of the child process (the full int written by wait())
- Takes the null-terminated string pointed to by cmd and evaluates as a single pipe command
- Forks
- Uses popen() to execute the two commands
- Uses clean_sep_line to separate by '|'
- Frees malloc from clean_sep_line call
```
void read_command(char * cmdbuffer);
```
- Takes a pointer cmdbuffer to a character buffer
- Prints a prompt consisting of the current working directory followed by '$' to STDOUT
- Uses fgets to read BUFFERSIZE characters from STDIN into the command buffer
```
int exec_single(char * cmd);
```
Returns: Either 0 or the exit status (the full int written by wait()) of the child
- Takes the null-terminated string pointed to by cmd and executes as a single command, calling my_pipe, redirect_out, redirect_in, redirect_double as necessary
- Separates command (once stripped of pipes and redirectons) by ' ' using sep_line
- Forks
- Uses execvp to execute command
- Frees malloc from sep_line call
```
void exec_all(char * cmds);
```
- Makes a null-terminated array of pointers to null-terminated command strings
- Runs exec_single for each command string
- Uses clean_sep to separate by ';'
- Frees the malloc from clean_sep call
