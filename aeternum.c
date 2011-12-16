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

  // Fork the process.
  int pid = fork();
  
  // fork() makes a clone of the current process - the same code will thus get
  // executed in both the parent and the child.  We must check for this.
  // fork() returns 0 in the child process, the child's pid in the parent
  // process, and -1 if an error occurs while forking.
  
  if (pid < 0) {
    // An error has occurred
    printf("An error has occurred while forking - errno %d.  Exiting.\n", errno);
    exit(pid);
  }
  else if (pid > 0) {
    // This is the parent process
    printf("Child has been spawned and daemonized. PID: %d\n", pid);
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

void run_target(const char *filename, char *args[]) {
  int result;
  result = execvp(filename, args);
  assert(result != -1);
}
