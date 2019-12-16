#include "shell.h"

int handle_errint(int value) {
    if (value == -1) {
        printf("Error #%d: %s\n", errno, strerror(errno));
    }
    return value;
}

void * handle_errpoint(void * value) {
    if (value == NULL) {
        printf("Error #%d: %s\n", errno, strerror(errno));
    }
    return value;
}

void read_command(char * cmdbuffer) {
    char cwd[CWDSIZE];
    handle_errpoint(getcwd(cwd, CWDSIZE));
    printf("%s$ ", cwd);
    fgets(cmdbuffer, BUFFERSIZE, stdin);
}

int exec_single(char * cmd) {
    int f = 0;

    if (strchr(cmd, '|')) {
        return my_pipe(clean_sep_line(cmd,'|'));
    }

    if (strchr(cmd, '<') && strchr(cmd, '>')){
        redirect_double(cmd);
        return f;
    } else if (strchr(cmd, '<')) {
        redirect_in(cmd);
        return f;
    } else if (strchr(cmd, '>')) {
        redirect_out(cmd);
        return f;
    }

    char ** argv = sep_line(cmd, ' ');
    if (strncmp(argv[0], "cd", 2) == 0) {
        chdir(argv[1]);
    } else {
        f = handle_errint(fork());
        if (f) {
            handle_errint(wait(&f));
        } else {
            exit(handle_errint(execvp(argv[0], argv)));
        }
    }
    free(argv);
    return f;
}

static void keyboard_interupt(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

void redirect_double(char * cmd){
    char ** argsin = clean_sep_line(cmd, '<');

    char ** argsout = clean_sep_line(argsin[1], '>');
    char * filein = argsout[0];
    char * fileout = argsout[1];

    int fdin = handle_errint(open(filein, O_RDWR | O_CREAT,  0640));
    int fdout = handle_errint(open(fileout, O_RDWR | O_CREAT | O_TRUNC,  0640));

    int backupout = dup(STDOUT_FILENO);
    handle_errint(dup2(fdout,STDOUT_FILENO));
    int backupin = dup(STDIN_FILENO);
    handle_errint(dup2(fdin,STDIN_FILENO));

    exec_single(argsin[0]);
    
    handle_errint(dup2(backupin, 0));
    handle_errint(dup2(backupout, 1));
    handle_errint(close(fdin));
    handle_errint(close(backupin));
    handle_errint(close(fdout));
    handle_errint(close(backupout));
    free(argsin);
    free(argsout);
}

void redirect_out(char * cmd){
    char ** args = clean_sep_line(cmd, '>');
    char * file = args[1];

    int fd = handle_errint(open(file, O_RDWR | O_CREAT | O_TRUNC,  0640));

    int backup = handle_errint(dup(STDOUT_FILENO));
    handle_errint(dup2(fd,STDOUT_FILENO));

    exec_single(args[0]);

    handle_errint(dup2(backup,1));
    handle_errint(close(fd));
    handle_errint(close(backup));
    
    free(args);
}

void redirect_in(char * cmd){
    char ** args = clean_sep_line(cmd, '<');
    char * file = args[1];

    int fd = handle_errint(open(file, O_RDWR | O_CREAT,  0640));

    int backup = dup(STDIN_FILENO);
    handle_errint(dup2(fd, STDIN_FILENO));
    
    exec_single(args[0]);

    handle_errint(dup2(backup,STDIN_FILENO));
    handle_errint(close(fd));
    handle_errint(close(backup));
    free(args);
}

int my_pipe(char ** cmdv) {
    int f = handle_errint(fork());
    if (f) {
        handle_errint(wait(&f));
        return f;
    } else {
        FILE * pipe_from = popen(cmdv[0],"r");
        handle_errpoint(pipe_from);
        FILE * pipe_to = popen(cmdv[1],"w");
        handle_errpoint(pipe_to);

        char buf;
        while ((buf = getc(pipe_from)) != EOF) {
            putc(buf, pipe_to);
        }
        exit(f);
    }
}

void exec_all(char * cmds) {
    char ** cmdv = clean_sep_line(cmds, ';');
    for (int i = 0; cmdv[i] != NULL; i++) {
        if (strncmp(cmdv[i], "exit", 4) == 0) {
            free(cmdv);
            exit(0);
        } else {
            exec_single(cmdv[i]);
        }
    }
    free(cmdv);
}
