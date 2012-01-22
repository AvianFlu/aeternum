#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "options.h"

struct Options *Options_parse(int argc, char *argv[]) {
  assert(argc > 1);

  Options *opts = malloc(sizeof(Options));
  assert(opts != NULL);

  opts->outfile = NULL;
  opts->errfile = NULL;
  opts->target = NULL;
  opts->child_args = NULL;

  int i;
  for (i = 1; i < argc; i++) {
    switch((int)argv[i][0]) {
      case '-':
        switch((int)argv[i][1]) {
          case 'o':
            if (argv[i + 1][0] != '-') {
              opts->outfile = strdup(argv[i + 1]);
            }
            break;
          case 'e':
            if (argv[i + 1][0] != '-') {
              opts->errfile = strdup(argv[i + 1]);
            }
            break;
          case '-':
            if (argv[i + 1] != NULL) {
              opts->target = strdup(argv[i + 1]);
              opts->child_args = &argv[i + 1];
            }
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
  return opts;
}

