#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>


void run_target(const char *name, char *args[]);

int main(int argc, char *argv[]) {
  assert(argc > 1);
  char **child_args = &argv[1];
  int i;
  printf("Aeternum started, daemonizing %s...\n", argv[2]);

  // Fork the process.
  int pid = fork();
  if (pid == 0) {
    // `fork(2)` returns 0 to the child process...
    printf("Fork successful...\n");
  }
  else if (pid < 0) {
    // < 0 in case of failure
    printf("An error has occurred while forking - errno %d.  Exiting.\n", errno);
    exit(pid);
  }
  else {
    // And PID of the child process to the parent
    printf("Aeternum parent exiting (child was spawned as %d)\n", pid);
    // We want to daemonize, so the parent needs to exit at this point.
    exit(0);
  }

  // Only child process can get here, parent process quits.
  
  // Detach from the terminal; start a new process group
  setsid();

  // Set the umask
  umask(027);

  // Redirect stdio to a file (only suitable if you want logging)
  int out = open("out.log", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  int err = open("err.log", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  dup2(out, STDOUT_FILENO);
  dup2(err, STDERR_FILENO);

  // Ignore a few signals that could otherwise ruin our day.
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);
  
  run_target(argv[1], child_args);
}

void run_target(const char *filename, char *args[])
{
  int result;
  result = execvp(filename, args);
  assert(result != -1);
  fprintf(stdout, "execvp() successful.");
}
