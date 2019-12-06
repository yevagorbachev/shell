#include "shell.h"

char ** sep_line(char * line, char * delim) {
    char ** argv = calloc(sizeof(char *), 6);
    int i = 0;
    while ((argv[i++] = strsep(&line, delim)) != NULL) {}
    return argv;
}

void read_command(char * cmdbuffer) {
    printf(">");
    fgets(cmdbuffer, BUFFERSIZE, stdin);

}

int exec_single(char * cmd) {
    int f = 0;
    char ** argv = sep_line(cmd, " "); // MALLOC 1
    if (strncmp(argv[0], "cd", 2) == 0) {
        chdir(argv[1]);
    } else {
        f = fork();
        if (f) {
            wait(&f);
        } else {
            exit(execvp(argv[0], argv));
        }
    }
    free(argv); // FREE 1
    return f;
}

void exec_all(char * cmds) {
    char ** cmdv = sep_line(cmds, ";"); // MALLOC 1
    for (int i = 0; cmdv[i] != NULL; i++){
        if (strncmp(cmdv[i], "exit", 4) == 0) {
            free(cmdv); // FREE 1 CASE 1
            exit(0);
        } else {
            exec_single(cmdv[i]);
        }
    }
    free(cmdv); // FREE 1 CASE 2
}
