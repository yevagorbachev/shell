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

int exec_command(char * cmd) {
    char ** argv = sep_line(cmd, " ");
    int f = fork();
    if (f) {
        wait(&f);
        free(argv);
        return f;
    } else {
        if (execvp(argv[0], argv) == -1){
          exit(0);
        }
    }
    return 0;
}

int exec_cd(char * cmd){
    char ** argv = sep_line(cmd, " ");
    chdir(argv[1]);
    return 0;
}

void exec_all_commands(char ** cmds) {
    for (int i = 0; cmds[i] != NULL; i++){
      if (strncmp(cmds[i], "cd", 2) == 0){
        exec_cd(cmds[i]);
      } else {
        exec_command(cmds[i]);
      }
    }
}
