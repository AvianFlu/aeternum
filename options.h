#ifndef _options_h
#define _options_h

struct Options {
  char *outfile;
  char *errfile;
  char *target;
  int json;
  char **child_args;
};

typedef struct Options Options;

struct Options *Options_parse(int argc, char *argv[]);

#endif
