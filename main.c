#include "shell.h"

int main() {
    char cmdbuffer[BUFFERSIZE];
    cmdbuffer[0] = '\0';
    
    while (strcmp(cmdbuffer, "exit") != 0) {
        read_command(cmdbuffer);
        cmdbuffer[strlen(cmdbuffer) - 1] = '\0';
        char ** commands = sep_line(cmdbuffer, ";");
        exec_all_commands(commands);
    }

}
