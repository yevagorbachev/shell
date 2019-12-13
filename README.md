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
- Only one space at most is allowed between commands and functions or the shell will break
- Semicolon after command will produce an errno
- Control-C will exit shell even if there is a function currently running

## Files & Function Headers
### parsing.c
Handles all line parsing functions
```
char * clean_sep(char ** line, char delim); 
```
Returns:
Functions like strsep but replaces spaces surrounding the single delimiter with nulls
```
char ** sep_line(char * line, char * delim);
```
Returns: array
Separates line by delim
Allocates heap memory
```
char ** clean_sep_line(char * line, char delim);
```
Returns: 
Like sep_line but uses clean_sep instead of strsep, on a single-character delimiter
Allocates heap memory

### shell.c
Handles the forking execution of commands
```
void redirect_out(char * cmd);
```
Returns: 

```
void redirect_in(char * cmd);
```
Returns:

```
int my_pipe(char ** cmdv);
```
Returns:

```
void read_command(char * cmdbuffer); // fgets BUFFERSIZE characters, writes to cmdbuffer
```
Returns:

```
int exec_single(char * cmd); // makes cmd an argv through sep_line(," "), executes argv
```
Returns:

```
// MALLOC (from sep_line) + FREE
void exec_all(char * cmds); // makes cmds a cmdv through sep_line(,";"), exec_single each in cmdv
```
Returns:

```
// MALLOC 1 (from sep_line) + FREE 1
// MALLOC 2 (from exec_single) + FREE 2
void redirect_double(char * cmd); //redirects both STDOUT and STDIN
```
Returns:

```
void redirect_out(char * cmd); //redirects STDOUT to a file and overwrites existing contens
```
Returns:

```
void redirect_in(char * cmd); //redirects STDIN from a file and replaces it
```
Returns:
