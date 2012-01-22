CFLAGS=-Wall

all: aeternum

aeternum: main.o libaeternum.a options.o
	cc main.o options.o -L. -laeternum -o aeternum

libaeternum.a: aeternum.o
	ar rcs libaeternum.a aeternum.o

aeternum.o:
	cc -c aeternum.c

main.o:
	cc -c main.c

options.o:
	cc -c options.c

clean:
	rm -f aeternum *.o libaeternum.a

install:
	cp aeternum /usr/local/bin/aeternum

uninstall:
	rm -rf /usr/local/bin/aeternum
