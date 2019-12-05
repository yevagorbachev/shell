# Shell Project

## Features List
### Functional
- Reading one line at a time, parsing the line to separate a command from its arguments
- Forks and executes the command
  - The parent process waits until the executed program exits before reading the next command
- Reads and separates multiple commands on one line with ;
  - ls -l;echo hello 
  - First runs the command ls -l and then echo hello
- cd command traverses through directories
- exit command ends the shell program

### Nonfunctional
- 

## Bugs
-

## Functions
```
char ** sep_line(char * line, char * delim);
void read_command(char * cmdbuffer);
int exec_command(char * cmd);
void exec_all_commands(char ** cmds);
int exec_cd(char * cmd);
```
