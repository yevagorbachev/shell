#include "shell.h"

char ** sep_line(char * line, char * delim) {
    char ** argv = calloc(sizeof(char *), 20);
    int i = 0;
    while ((argv[i++] = strsep(&line, delim)) != NULL) {}
    return argv;
}


char * clean_sep(char ** line, char delim) {
    char * start = *line;
    char * next = *line;
    if ((next != NULL) && ((next = strchr(next, delim)))) {
        *next = '\0';
        if (*(next - 1) == ' ') {
            *(next - 1) = '\0';
        }
        if (*(++next) == ' ') {
            *(next++) = '\0';
        }
    }
    *line = next;
    return start;
}


char ** clean_sep_line(char * line, char delim) {
    char ** argv = calloc(sizeof(char *), 20);
    int i = 0;
    while ((argv[i++] = clean_sep(&line, delim)) != NULL) {}
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

void exec_all(char * cmds) {
    char ** cmdv = clean_sep_line(cmds, ';'); // MALLOC 1
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
