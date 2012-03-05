#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "options.h"

options_t options_parse(int argc, char *argv[]) {
  options_t opts;
  int i;

  assert(argc > 1);

  opts.outfile = NULL;
  opts.errfile = NULL;
  opts.json = 0;
  opts.num_args = 0;
  opts.child_args = NULL;
  opts.target = NULL;
  opts.command = NULL;

  for (i = 1; i < argc; i++) {
    switch((int)argv[i][0]) {
      case '-':
        switch((int)argv[i][1]) {
          case 'o':
            if (argv[i + 1][0] != '-') {
              opts.outfile = &argv[i + 1][0];
            }
            break;
          case 'e':
            if (argv[i + 1][0] != '-') {
              opts.errfile = &argv[i + 1][0];
            }
            break;
          case 'j':
            opts.json = 1;
            break;
          case '-':
            if (argv[i + 1] != NULL) {
              opts.target = &argv[i + 1][0];
              opts.child_args = &argv[i + 1];
              opts.num_args = argc-i-1;
              return opts;
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
