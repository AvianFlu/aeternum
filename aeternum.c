#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include "aeternum.h"

struct Options *aeternum_options(int argc, char *argv[]) {
  Options *opts = Options_parse(argc, argv);
  return opts;
}

void aeternum_start(Options *opts) {
  assert(opts != NULL);
  assert(opts->outfile != NULL);
  assert(opts->errfile != NULL);
  assert(opts->child_args);
  aeternum_fork();
  aeternum_redirect(opts->outfile, STDOUT_FILENO);
  aeternum_redirect(opts->errfile, STDERR_FILENO);
  aeternum_exec(opts->target, opts->child_args);
}

void aeternum_fork() {

  int pid = fork();

  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  if (pid < 0) {
    printf("An error occurred: %s", strerror(errno));
    // Should we find a graceful way to bail here?
    exit(pid);
  }
  else if (pid > 0) {
    printf("Child has been spawned and daemonized. PID: %d\n", pid);
    exit(0);
  }

  setsid();
  umask(027);
}

void aeternum_redirect(const char *dest, int fileno) {
  int out = open(dest,
                  O_WRONLY | O_APPEND | O_CREAT,
                  S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  if (out == -1) printf("An error occurred: %s", strerror(errno));
  assert(out != -1);
  int dup_res = dup2(out, fileno);
  if (dup_res == -1) printf("An error occurred: %s", strerror(errno));
  assert(dup_res != -1);
}

void aeternum_exec(const char *filename, char *args[]) {
  int result = execvp(filename, args);
  assert(result != -1);
}
