#include "aeternum.h"

int main(int argc, char *argv[]) {
  void *opts = aeternum_options(argc, argv);
  aeternum_start(opts);
  return 0;
}


