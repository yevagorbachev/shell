#include "shell.h"

int main() {
    char cmdbuffer[100];
    cmdbuffer[0] = '\0';

    char * test1 = "ls -a";

    // char * test2 = "cd .git";

    while (strcmp(cmdbuffer, "exit") != 0) {
        read_command(cmdbuffer);
        cmdbuffer[strlen(cmdbuffer) - 1] = '\0';
        exec_command(cmdbuffer);
    }

}