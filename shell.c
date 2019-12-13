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
    if(strchr(cmd,'>')){
        //printf("%s\n", argv[i]);
        redirect_out(cmd);
        return f;
    }
    if (strchr(cmd,'<')){
        //printf("%s\n", argv[i]);
        redirect_in(cmd);
        return f;
    }

    char ** argv = sep_line(cmd, " "); // MALLOC 1
    if (strncmp(argv[0], "cd", 2) == 0) { // special case - no fork for cd
        chdir(argv[1]);
    } else {
        f = handle_errint(fork());

        if (f) {
            handle_errint(wait(&f));
        } else {
            exit(handle_errint(execvp(argv[0], argv))); // if execvp fails, exits anyway
        }
    }
    free(argv); // FREE 1
    return f;
}

void redirect_out(char * cmd){
  char ** args = clean_sep_line(cmd, '>');
  char ** argv = sep_line(args[0], " ");
  char * file = args[1];

  int fd = handle_errint(open(file, O_RDWR | O_CREAT | O_TRUNC,  0640)); // ERRNO

  int backup = handle_errint(dup(STDOUT_FILENO));
  handle_errint(dup2(fd,STDOUT_FILENO));

  int f = handle_errint(fork());
  if (f) {
      handle_errint(wait(&f));
      handle_errint(dup2(backup,1));

      handle_errint(close(fd));
      handle_errint(close(backup));
  } else {
      exit(handle_errint(execvp(argv[0], argv))); // if execvp fails, exits anyway
  }
}

void redirect_in(char * cmd){
  char ** args = clean_sep_line(cmd, '<');
  char ** argv = sep_line(args[0], " ");
  char * file = args[1];

  int fd = handle_errint(open(file, O_RDWR | O_CREAT,  0640));

  int backup = handle_errint(dup(STDIN_FILENO));
  handle_errint(dup2(fd,0));

  int f = handle_errint(fork());
  if (f) {
      handle_errint(wait(&f));
      handle_errint(dup2(backup,STDIN_FILENO));
      handle_errint(close(fd));
      handle_errint(close(backup));
  } else {
      exit(handle_errint(execvp(argv[0], argv)));
  }

}

int my_pipe(char ** cmdv) {
    int f = fork(); // ERRNO
    if (f) {
        handle_errint(wait(&f)); // ERRNO
        return f;
    } else {
        FILE * pipe_from = popen(cmdv[0],"r"); // ERRNO NULL
        handle_errpoint(pipe_from);
        FILE * pipe_to = popen(cmdv[1],"w"); // ERRNO NULL
        handle_errpoint(pipe_to);

        char buf;
        while ((buf = getc(pipe_from)) != EOF) {
            putc(buf, pipe_to);
        }
        exit(f);
    }
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
