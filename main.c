#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "aeternum.h"
#include "options.h"

int main(int argc, char *argv[]) {
  void *opts = aeternum_options(argc, argv);
  aeternum_start(opts);
  return 0;
}


