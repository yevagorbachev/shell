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
4. At most 19 commands per line can be taken, with at most 19 tokens per command.

## Files & Function Headers
### parsing.c
Handles all line parsing functions
```
char * clean_sep(char ** line, char delim); 
```
Returns: Pointer to the beginning of line<br />
Functions like strsep but replaces spaces surrounding the single delimiter with nulls
```
char ** sep_line(char * line, char * delim);
```
Returns: Array of strings where each entry is a token<br />
Separates line by delimiter<br />
Allocates heap memory
```
char ** clean_sep_line(char * line, char delim);
```
Returns: Array of strings where each entry is a token<br />
Like sep_line but uses clean_sep instead of strsep, on a single-character delimiter<br />
Allocates heap memory

### shell.c
Handles the forking and execution of commands
```
int my_pipe(char ** cmdv);
```
Returns: Value of fork()<br />
Takes in a commands with both a pipe and executes it
```
void read_command(char * cmdbuffer);
```
fgets BUFFERSIZE characters and writes to cmdbuffer
```
int exec_single(char * cmd);
```
Returns: Value of fork()<br />
Makes cmd an argv through sep_line(," "), executes argv
```
void exec_all(char * cmds);
```
Makes cmds a cmdv through sep_line(,";"), exec_single each in cmdv
```
void redirect_double(char * cmd); //redirects both STDOUT and STDIN
```
Takes in a commands with both a redirect out and a redirect in and executes it
```
void redirect_out(char * cmd);
```
Takes in a commands with a redirect out and executes it
```
void redirect_in(char * cmd);
```
Takes in a commands with a redirect in and executes it
