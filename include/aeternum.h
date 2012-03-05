#ifndef _aeternum_h
#define _aeternum_h

#include "options.h"

options_t aeternum_options(int argc, char *argv[]);

char* aeternum_get_error(void);

void aeternum_start(options_t opts);

#endif
