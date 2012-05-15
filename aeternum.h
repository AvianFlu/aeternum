#ifdef __cplusplus
extern "C" {
#endif

#ifndef _aeternum_h
#define _aeternum_h

#include "options.h"

options_t aeternum_options(int argc, char *argv[]);

void aeternum_start(options_t opts);

int aeternum_fork();

int aeternum_redirect(const char *filename, int fileno);

int aeternum_dup(int oldfd, int newfd);

void aeternum_exec(const char *filename, char *args[]);

#endif

#ifdef __cplusplus
}
#endif
