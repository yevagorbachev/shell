#include "shell.h"

int main() {
    char cmdbuffer[BUFFERSIZE];
    cmdbuffer[0] = '\0';

    while (1) {
        read_command(cmdbuffer);
        cmdbuffer[strlen(cmdbuffer) - 1] = '\0';
        exec_all(cmdbuffer);
    }

}
