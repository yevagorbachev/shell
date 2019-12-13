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

    // int count = 0;
    //
    // for (int i =0; cmd[i] != 0; i++){
    //   printf("%c\n",cmd[i]);
    //   if( cmd[i] == '>' || cmd[i] == '<'){
    //     count++;
    //   }
    // }

    if (strchr(cmd, '<') && strchr(cmd, '>')){
      redirect_double(cmd);
      return f;
    }

    if (strchr(cmd, '<')) {
      redirect_in(cmd);
      return f;
    }

    if (strchr(cmd, '>')) {
      redirect_out(cmd);
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

static void keyboard_interupt(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

void redirect_double(char * cmd){
  char ** argsin = clean_sep_line(cmd, '<');
  char ** argvin = sep_line(argsin[0], " ");

  char ** argsout = clean_sep_line(argsin[1], '>');
  char ** argvout = sep_line(argsout[0], " ");
  char * filein = argsout[0];
  // printf("%s\n", filein);
  char * fileout = argsout[1];
  // printf("%s\n", fileout);

  int fdin = open(filein, O_RDWR | O_CREAT,  0640);
  if (fdin == -1){
    printf("Error opeing file: %s\n", strerror(errno));
  }
  int fdout = open(fileout, O_RDWR | O_CREAT | O_TRUNC,  0640);
  if (fdout == -1){
    printf("Error opeing file: %s\n", strerror(errno));
  }

  int backupout = dup(STDOUT_FILENO);
  if (dup2(fdout,STDOUT_FILENO) < 0){
    printf("dup2 error: %s\n",strerror(errno));
  }
  int backupin = dup(STDIN_FILENO);
  if (dup2(fdin,STDIN_FILENO) < 0){
    printf("dup2 error: %s\n",strerror(errno));
  }

  int f = fork();
  if (f) {
      //printf("!!!!!!!!!!!\n");
      wait(&f);
      if (dup2(backupin,0) < 0){
        printf("dup2 error: %s\n",strerror(errno));
      }
      if (dup2(backupout,1) < 0){
        printf("dup2 error: %s\n",strerror(errno));
      }
      close(fdin);
      close(backupin);
      close(fdout);
      close(backupout);
  } else {
      //printf("HEEEERE\n");
      exit(execvp(argvin[0], argvin)); // if execvp fails, exits anyway
  }
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

  int backup = dup(STDIN_FILENO);
  if (dup2(fd,STDIN_FILENO) < 0){
    printf("dup2 error: %s\n",strerror(errno));
  }

  int f = fork();
  if (f) {
      wait(&f);
      if (dup2(backup,STDIN_FILENO) < 0){
        printf("dup2 error: %s\n",strerror(errno));
      }
      close(fd);
      close(backup);
  } else {
      exit(execvp(argv[0], argv)); // if execvp fails, exits anyway
  }

    f = handle_errint(fork());
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
