CFLAGS= -W -Wall -g -std=c11 -D_DEFAULT_SOURCE 
PROGS= maptest
OBJECTS=hashmap.o maptest.o

all: $(PROGS)

maptest: maptest.o hashmap.o
	cc -o maptest $^


hashmap.o: hashmap.h hashmap.c
maptest.o: maptest.c


clean:
	rm -f *~ $(OBJECTS) $(PROGS)