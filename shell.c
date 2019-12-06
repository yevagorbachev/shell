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
    char ** argv = sep_line(cmd, " ");
    if (strncmp(argv[0], "exit", 4) == 0) {
        exit(0);
    }
    else if (strncmp(argv[0], "cd", 2) == 0){
        chdir(argv[1]);
        return 0;
    }
    int f = fork();
    if (f) {
        wait(&f);
        free(argv);
        return f;
    } else {
        exit(execvp(argv[0], argv));
    }
    return 0;
}

void exec_all(char ** cmds) {
    for (int i = 0; cmds[i] != NULL; i++){
        exec_single(cmds[i]);
    }
}
