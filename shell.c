#include "shell.h"

char ** sep_line(char * line, char * delim) {
    char ** argv = calloc(sizeof(char *), 6);
    int i = 0;
    while ((argv[i++] = strsep(&line, delim)) != NULL) {}
    return argv;
}

void read_command(char * cmdbuffer) {
    printf(">");
    fgets(cmdbuffer, 100, stdin);

}

int exec_command(char * cmd) {
    char ** argv = sep_line(cmd, " ");
    int f = fork();
    if (f) {
        wait(&f);
        free(argv);
        return f;
    } else {
        execvp(argv[0], argv);
    }
}