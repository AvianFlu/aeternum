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

static int json = 0;
static int silent = 1;

options_t aeternum_options(int argc, char *argv[]) {
  options_t opts = options_parse(argc, argv);
  return opts;
}

void aeternum_start(options_t opts) {
  assert(opts.target != NULL);
  assert(opts.child_args != NULL);

  if (opts.json) json = 1;
  if (!opts.silent) silent = 0;

  if (aeternum_fork() < 0) {
    fprintf(stderr, "fork(), %s", strerror(errno));
    exit(1);
  }

  if (aeternum_dup(-1, STDIN_FILENO) < 0) {
    fprintf(stderr, "stdin redirect, %s", strerror(errno));
    exit(1);
  }
  if (aeternum_redirect(opts.outfile, STDOUT_FILENO) < 0) {
    fprintf(stderr, "stdout redirect, %s", strerror(errno));
    exit(1);
  }
  if (aeternum_redirect(opts.errfile, STDERR_FILENO) < 0) {
    fprintf(stderr, "stderr redirect, %s", strerror(errno));
    exit(1);
  }
  aeternum_exec(opts.target, opts.child_args);
}

int aeternum_fork() {
  int pid = -1;
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  pid = fork();

  if (pid < 0) {
    if (json) {
      fprintf(stderr, "{\"error\": \"%s\"}", strerror(errno));
    }
    else {
      fprintf(stderr, "An error occurred: %s", strerror(errno));
    }
    return pid;
  }
  else if (pid > 0) {
    if (json && !silent) {
      printf("{\"pid\": %d}", pid);
    }
    else if (!silent) {
      printf("Child has been spawned and daemonized. PID: %d\n", pid);
    }
    exit(0);
  }

  if (setsid() == -1) {
    return -1;
  }
  umask(027);
  return 0;
}

int aeternum_redirect(const char *dest, int fileno) {
  int out;
  const char *filename;

  if (dest == NULL) {
    filename = "/dev/null";
  }
  else {
    filename = dest;
  }
  out = open(filename, O_WRONLY | O_APPEND | O_CREAT,
                  S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
  if (out == -1) {
    return -1;
  }
  if (dup2(out, fileno) == -1) {
    return -1;
  }
  return 0;
}

int aeternum_dup(int oldfd, int newfd) {
  if (oldfd == -1) {
    oldfd = open("/dev/null", newfd  == 0 ? O_RDONLY : O_WRONLY);
  }
  if (close(newfd) == -1) {
    return -1;
  }
  if (dup2(oldfd, newfd) == -1) {
    return -1;
  }
  return 0;
}

void aeternum_exec(const char *filename, char *args[]) {
  if (execvp(filename, args) != 0) {
    perror("execvp()");
    exit(errno);
  }
}

