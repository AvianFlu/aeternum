## Aeternum

This is a very small C program that will daemonize any process on a Linux
system.  All command line arguments aside from the name of the program to execute are passed
through to the child.

### Installation

     git clone git@github.com:AvianFlu/Aeternum.git
     cd Aeternum
     make
     sudo make install

This should make `aeternum` available on your PATH.

### Usage

     aeternum [program] [args...]


