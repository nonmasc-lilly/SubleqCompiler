ifeq ($(origin CC), default)
CC = gcc
endif
CFLAGS ?= -c -std=c90 -Wpedantic -ffunction-sections -fdata-sections
LFLAGS ?= -ffunction-sections -fdata-sections -s
OFLAGS ?= -o

all: build/subleqc clean

build/subleqc: build/main.o build/process.o build/compile.o
	$(CC) $(LFLAGS) $(OFLAGS) $@ $^

build/main.o: src/main.c
	$(CC) $(CFLAGS) $(OFLAGS) $@ $<

build/process.o: src/process.c
	$(CC) $(CFLAGS) $(OFLAGS) $@ $<

build/compile.o: src/compile.c
	$(CC) $(CFLAGS) $(OFLAGS) $@ $<


test: all FORCE
	build/subleqc test/hi.subleq test/hi.asm -d

clean: FORCE
	rm build/*.o

FORCE:
