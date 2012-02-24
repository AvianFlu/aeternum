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

static int json;

struct Options *aeternum_options(int argc, char *argv[]) {
  Options *opts = Options_parse(argc, argv);
  return opts;
}

void aeternum_start(Options *opts) {
  assert(opts != NULL);
  assert(opts->target != NULL);
  assert(opts->child_args != NULL);
  if (opts->json) json = 1;
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
    if (json) {
      printf("{\"error\": \"%s\"}", strerror(errno));
    }
    else {
      printf("An error occurred: %s", strerror(errno));
    }
    exit(pid);
  }
  else if (pid > 0) {
    if (json) {
      printf("{\"pid\": %d}", pid);
    }
    else {
      printf("Child has been spawned and daemonized. PID: %d\n", pid);
    }
    exit(0);
  }

  setsid();
  umask(027);
}

void aeternum_redirect(const char *dest, int fileno) {
  if (dest == NULL) {
    dest = "/dev/null\0";
  }
  int out = open(dest, O_WRONLY | O_APPEND | O_CREAT,
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
