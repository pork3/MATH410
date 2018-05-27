CFLAGS= -W -Wall -g -std=c11 -D_DEFAULT_SOURCE 
PROGS= maptest
OBJECTS=hash.o hashfxn.o maptest.o 

all: $(PROGS)

maptest: maptest.o hash.o hashfxn.o
	cc -o maptest $^


hash.o: hash.c hash.h
maptest.o: maptest.c
hashfxn.o: hashfxn.c hashfxn.h


clean:
	rm -f *~ $(OBJECTS) $(PROGS)