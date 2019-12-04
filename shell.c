#include "shell.h"

char ** parse_args(char * line) {
    char ** argv = calloc(sizeof(char *), 6);
    int i = 0;
    while ((argv[i++] = strsep(&line, " ")) != NULL) {}
    return argv;
}

int read_command(char * cmdbuffer) {
    printf(">");
    fgets(cmdbuffer, 100, stdin);
}

int exec_command(char * cmd) {
    char ** argv = parse_args(cmd);
    int f = fork();
    if (f) {
        wait(&f);
        free(argv);
    } else {
        execvp(argv[0], argv);
    }
    wait(f);
}