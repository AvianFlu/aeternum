#ifndef _options_h
#define _options_h

struct options_s {
  char *outfile;
  char *errfile;
  int json;
  int num_args;

  //For Unix
  char **child_args;
  char *target;

  //For Windows
  char* command;
};

typedef struct options_s options_t;

options_t options_parse(int argc, char *argv[]);

#endif
