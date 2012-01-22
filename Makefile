CFLAGS=-Wall

all: aeternum

aeternum: main.c aeternum.c options.c

clean:
	rm -f aeternum

install:
	cp aeternum /usr/local/bin/aeternum

uninstall:
	rm -rf /usr/local/bin/aeternum
