# Makefile for Xenomai Hello, world...
CFLAGS := $(shell xeno-config --posix --cflags) -Werror=implicit-function-declaration -I../spkr-kernel-module/
LDFLAGS := $(shell xeno-config --posix --ldflags)

PROG := spkr-app

all : $(PROG)

$(PROG) : $(PROG).c

run: $(PROG)
	./$(PROG)

clean:
	rm -f -- $(PROG)

.PHONY: all run clean

