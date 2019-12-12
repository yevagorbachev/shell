#include "shell.h"
void redirect_out(char * cmd);
void redirect_in(char * cmd);
int my_pipe(char ** cmdv);


void read_command(char * cmdbuffer) {
    char cwd[CWDSIZE];
    getcwd(cwd, CWDSIZE);
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

static void keyboard_interupt(int signo){
  if (signo == SIGINT){
    exit(0);
  }
}

void redirect_out(char * cmd){
  char ** args = clean_sep_line(cmd, '>');
  char ** argv = sep_line(args[0], " ");
  char * file = args[1];

  int fd = open(file, O_RDWR | O_CREAT | O_TRUNC,  0640);
  if (fd == -1){
    printf("Error opeing file: %s\n", strerror(errno));
  }

  int backup = dup(STDOUT_FILENO);
  if (dup2(fd,STDOUT_FILENO) < 0){
    printf("dup2 error: %s\n",strerror(errno));
  }

  int f = fork();
  if (f) {
      //printf("!!!!!!!!!!!\n");
      wait(&f);
      if (dup2(backup,1) < 0){
        printf("dup2 error: %s\n",strerror(errno));
      }
      close(fd);
      close(backup);
  } else {
      //printf("HEEEERE\n");
      exit(execvp(argv[0], argv)); // if execvp fails, exits anyway
  }
}

void redirect_in(char * cmd){
  char ** args = clean_sep_line(cmd, '<');
  char ** argv = sep_line(args[0], " ");
  char * file = args[1];

  int fd = open(file, O_RDWR | O_CREAT,  0640);
  if (fd == -1){
    printf("Error opeing file: %s\n", strerror(errno));
  }

  int backup = dup(STDIN_FILENO);
  if (dup2(fd,0) < 0){
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

}

int my_pipe(char ** cmdv) {
    int f = fork();
    if (f) {
        wait(&f);
        return f;
    } else {
        FILE * pipe_from = popen(cmdv[0],"r");
        FILE * pipe_to = popen(cmdv[1],"w");
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
