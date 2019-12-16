# SHELL

## Features
- Reading one line at a time, parsing the line to separate a command from its arguments
- Forks and executes the command
  - The parent process waits until the executed program exits before reading the next command
- Reads and separates multiple commands on one line with ;
  - ls -l;echo hello 
  - First runs the command ls -l and then echo hello
- cd command traverses through directories
- Simple redirection with one > or <
  - w > wholist or tr a-z A-Z < wholist
- Double redirection with < then >
  - tr a-z A-Z < wholist > foo
- Simple piping with one |
  - ls | wc
- exit command ends the shell program

## Attempted
- 

## Bugs
- Only one whitespace at most is allowed between commands and functions ( |, <, >, ; ) or the shell will break
- Semicolon after command will produce an errno
- Control-C will exit shell even if there is a function currently running

## Files & Function Headers
### parsing.c
Handles all line parsing functions
```
char * clean_sep(char ** line, char delim); 
```
Returns: Pointer to the beginning of line
Functions like strsep but replaces spaces surrounding the single delimiter with nulls
```
char ** sep_line(char * line, char * delim);
```
Returns: Array of strings where each entry is a token
Separates line by delimiter
Allocates heap memory
```
char ** clean_sep_line(char * line, char delim);
```
Returns: Array of strings where each entry is a token
Like sep_line but uses clean_sep instead of strsep, on a single-character delimiter
Allocates heap memory

### shell.c
Handles the forking and execution of commands
```
int my_pipe(char ** cmdv);
```
Returns: Value of fork()
Takes in a commands with both a pipe and executes it
```
void read_command(char * cmdbuffer);
```
fgets BUFFERSIZE characters and writes to cmdbuffer
```
int exec_single(char * cmd);
```
Returns: Value of fork()
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
