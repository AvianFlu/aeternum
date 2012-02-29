#ifndef _aeternum_h
#define _aeternum_h

#include "options.h"

options_t aeternum_options(int argc, char *argv[]);

void aeternum_start(options_t opts);

void aeternum_fork();

void aeternum_redirect(const char *filename, int fileno);

void aeternum_exec(const char *filename, char *args[]);

#endif
