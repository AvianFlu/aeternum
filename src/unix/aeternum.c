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

options_t aeternum_options(int argc, char *argv[]) {
  options_t opts = options_parse(argc, argv);
  return opts;
}

void aeternum_start(options_t opts) {
  assert(opts.target != NULL);
  assert(opts.child_args != NULL);

  if (opts.json) json = 1;
  aeternum_fork();
  aeternum_redirect(opts.outfile, STDOUT_FILENO);
  aeternum_redirect(opts.errfile, STDERR_FILENO);
  aeternum_exec(opts.target, opts.child_args);
}

void aeternum_fork() {
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  int pid = fork();

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

  if (setsid() == -1) {
    perror("setsid()");
    exit(errno);
  }
  umask(027);
}

void aeternum_redirect(const char *dest, int fileno) {
  int out;

  if (dest == NULL) {
    dest = "/dev/null\0";
  }
  out = open(dest, O_WRONLY | O_APPEND | O_CREAT,
                  S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  if (out == -1) {
    perror("open()");
    exit(errno);
  }
  if (dup2(out, fileno) == -1) {
    perror("dup2()");
    exit(errno);
  }
}

void aeternum_exec(const char *filename, char *args[]) {
  if (execvp(filename, args) != 0) {
    perror("execvp()");
    exit(errno);
  }
}
