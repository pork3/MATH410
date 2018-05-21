CFLAGS= -W -Wall -g -std=c11 -D_DEFAULT_SOURCE 
PROGS= maptest
OBJECTS=hashmap.o maptest.o hashfunc.o

all: $(PROGS)

maptest: maptest.o hashmap.o hashfunc.o
	cc -o maptest $^


hashmap.o: hashmap.h hashmap.c
maptest.o: maptest.c
hashfunc.o: hashfunc.c hashfunc.h


clean:
	rm -f *~ $(OBJECTS) $(PROGS)