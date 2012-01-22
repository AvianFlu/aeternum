## aeternum

`aeternum` is a very small program that should daemonize any process on a Linux
system.  

### Installation

     git clone git@github.com:AvianFlu/aeternum.git
     cd aeternum
     make
     sudo make install

This should make `aeternum` available on your PATH.

### Usage

     aeternum -o [outfile] -e [errfile] -- [program] [args...]

`outfile` is a file path to which the program's `stdout` should be redirected.

`errfile` is a file path to which `stderr` should be redirected.

If either is not provided, `/dev/null` will be used.

The `--` option is used to delimit `aeternum` arguments from arguments to the
process being spawned.  Everything after `--` will be passed to `execvp()`.
