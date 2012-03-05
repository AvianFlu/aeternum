#ifndef _aeternum_unix_h
#define _aeternum_unix_h

void aeternum_fork();

void aeternum_redirect(const char *filename, int fileno);

void aeternum_exec(const char *filename, char *args[]);

#endif