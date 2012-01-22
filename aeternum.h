#ifndef _aeternum_h
#define _aeternum_h

#include "options.h"

struct Options *aeternum_options(int argc, char *argv[]);

void aeternum_start(Options *opts);

void aeternum_fork();

void aeternum_redirect(const char *filename, int fileno);

void aeternum_exec(const char *filename, char *args[]);

#endif
