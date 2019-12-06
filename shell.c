#include "shell.h"

char ** sep_line(char * line, char * delim) {
    char ** argv = calloc(sizeof(char *), 6);
    int i = 0;
    while ((argv[i++] = strsep(&line, delim)) != NULL) {
        // uses return value of assignment
    }
    return argv;
}

void read_command(char * cmdbuffer) {
    char cwd[CWDSIZE];
    getcwd(cwd, CWDSIZE);
    printf("%s>", cwd);
    fgets(cmdbuffer, BUFFERSIZE, stdin);

}

int exec_single(char * cmd) {
    int f = 0;
    char ** argv = sep_line(cmd, " "); // MALLOC 1
    if (strncmp(argv[0], "cd", 2) == 0) { // special case - no fork for cd
        chdir(argv[1]);
    } else {
        f = fork();
        if (f) {
            wait(&f);
        } else {
            exit(execvp(argv[0], argv)); // if execvp fails, exits anyway
        }
    }
    free(argv); // FREE 1
    return f;
}

//PLANNING FOR PIPING
//fork()
//popen(command, "w") always
//how to handle multiple?
//possible from tail:
//ls -l | xxd | wc evaluates as
// popen(wc, "w"); fork(); exec_single("ls -l | xxd");

void exec_all(char * cmds) {
    char ** cmdv = sep_line(cmds, ";"); // MALLOC 1
    for (int i = 0; cmdv[i] != NULL; i++) {
        if (strncmp(cmdv[i], "exit", 4) == 0) {
            free(cmdv); // FREE 1 CASE 1
            exit(0);
        } else {
            exec_single(cmdv[i]); // MALLOC 2 + FREE 2
        }
    }
    free(cmdv); // FREE 1 CASE 2
}
