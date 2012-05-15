#ifdef __cplusplus
extern "C" {
#endif

#ifndef _options_h
#define _options_h

struct options_s {
  char *outfile;
  char *errfile;
  char *target;
  int json;
  int silent;
  char **child_args;
};

typedef struct options_s options_t;

options_t options_parse(int argc, char *argv[]);

#endif

#ifdef __cplusplus
}
#endif
